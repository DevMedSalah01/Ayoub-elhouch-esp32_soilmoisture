#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <ArduinoJson.h>

// Paramètres WiFi
const char* ssid = "";
const char* password = "";

// Paramètres CoAP
const int coapPort = 5683;

// Paramètres REST API
const char* restApiUrl = "http://localhost:3001/";

// Capteurs
#include <DHT.h>
#define DHTPIN 21
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LED
const int LED_PIN = 4;

// Objet CoAP
WiFiUDP udp;
Coap coap(udp);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  setupCoAP();
}

void loop() {
  // Envoyer les données du capteur au serveur CoAP
  sendSensorDataToCoAPServer();

  // Vérifier si une commande a été envoyée par le serveur REST API
  checkAndExecuteLEDCommand();

  delay(10000); // Attendre 10 secondes avant la prochaine lecture
}

void sendSensorDataToCoAPServer() {
  float temperature = dht.readTemperature(false); // false for Celsius, true for Fahrenheit
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  CoapPacket packet;
  packet.type = COAP_CON;
  packet.code = COAP_POST;

  packet.addOption(COAP_URI_PATH, strlen("/sensor-data"), (uint8_t*)"/sensor-data");

  String payloadString = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
  char buffer[payloadString.length() + 1];
  payloadString.toCharArray(buffer, sizeof(buffer));

  packet.payload = (uint8_t*)buffer;
  packet.payloadlen = strlen(buffer);

  coap.send(WiFi.localIP(), coapPort, NULL, COAP_CON, COAP_POST, NULL, 0, packet.payload, packet.payloadlen);
}

void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  // Allocate a new buffer with the correct size
  char* p = new char[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = '\0'; // Ajouter le caractère null de fin de chaîne
  String message(p);

  if (message.equals("0")) {
    digitalWrite(LED_PIN, LOW);
  } else if (message.equals("1")) {
    digitalWrite(LED_PIN, HIGH);
  }

  coap.sendResponse(ip, port, packet.messageid, message.c_str());

  // Free the allocated memory
  delete[] p;
}

void setupCoAP() {
  coap.server(callback_light, "/led");
  coap.start(coapPort);
}

void checkAndExecuteLEDCommand() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(String(restApiUrl) + "/led");
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
      StaticJsonDocument<200> respDoc;
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