#include <WiFi.h>
#include <HTTPClient.h>
#include <CoAP.h>
#include <ArduinoJson.h>

// Paramètres WiFi
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Paramètres CoAP
const char* coapHost = "your_coap_server_ip";
const int coapPort = 5683;

// Paramètres REST API
const char* restApiHost = "your_rest_api_server_ip";
const int restApiPort = 3000;

// Capteurs
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED
const int LED_PIN = 2;

// Objet CoAP
CoAP coap;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  dht.begin();

  // Configurer le client CoAP
  coap.begin();
  coap.setServer(coapHost, coapPort);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Envoyer les données du capteur au serveur CoAP
  sendSensorDataToCoAPServer();

  // Vérifier si une commande a été envoyée par le serveur REST API
  checkAndExecuteLEDCommand();

  delay(10000); // Attendre 10 secondes avant la prochaine lecture
}

void sendSensorDataToCoAPServer() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    String requestBody;
    serializeJson(doc, requestBody);

    CoAPMessage message;
    message.setType(COAP_MSG_CON);
    message.setCode(COAP_POST);
    message.setURI("/sensor-data");
    message.setPayload(requestBody.c_str(), requestBody.length());

    int result = coap.send(message);
    if (result == COAP_OK) {
      Serial.println("Data sent successfully via CoAP");
    } else {
      Serial.print("Error sending data via CoAP: ");
      Serial.println(result);
    }
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void checkAndExecuteLEDCommand() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(String("http://") + restApiHost + ":" + restApiPort + "/led");
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["state"] = "on"; // Ou "off"
    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(http.getString());

      // Mettre à jour l'état de la LED en fonction de la réponse
      JsonDocument respDoc;
      DeserializationError error = deserializeJson(respDoc, http.getString());
      if (error) {
        Serial.print("Erreur de désérialisation JSON: ");
        Serial.println(error.c_str());
      } else {
        const char* ledState = respDoc["state"];
        handleLEDCommand(ledState);
      }
    } else {
      Serial.print("Error sending HTTP POST: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void handleLEDCommand(const char* state) {
  if (strcmp(state, "on") == 0) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED turned on");
  } else if (strcmp(state, "off") == 0) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED turned off");
  } else {
    Serial.println("Invalid LED state");
  }
}