#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Paramètres WiFi
const char* ssid = "";
const char* password = "";

// Capteur d'humidité du sol
const int soilMoisturePin = A0; // Pin pour le capteur d'humidité du sol

const int ledPin = 21; // LED intégrée sur l'ESP32

// AsyncWebServer instance
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Configurer la LED comme sortie
  pinMode(ledPin, OUTPUT);

  // Configurer les routes du serveur web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int soilMoistureValue1 = analogRead(soilMoisturePin);
    int soilMoistureValue = soilMoistureValue1/10;
    String html = "<html><head>";
html += "<style>";
html += "body { font-family: Arial, sans-serif; margin: 0; padding: 0; background-color: #f2f2f2; }";
html += "header { background-color: #333; color: #fff; text-align: center; padding: 10px 0; }";
html += "h1 { margin-top: 20px; text-align: center; }";
html += ".container { display: flex; justify-content: center; align-items: center; height: 80vh; }";
html += ".content { text-align: center; }";
html += ".button { background-color: #4caf50; border: none; color: white; padding: 15px 32px; text-align: center;";
html += "text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }";
html += ".button:hover { background-color: #45a049; }";
html += "</style>";
html += "</head><body>";
html += "<header><h1>Soil Moisture Monitoring</h1></header>";
html += "<div class='container'>";
html += "<div class='content'>";
html += "<h2>Soil Moisture: " + String(soilMoistureValue) + "</h2>";
html += "<p>Control LED:</p>";
html += "<button class='button' onclick=\"sendCommand(0)\">Turn Off</button>";
html += "<button class='button' onclick=\"sendCommand(1)\">Turn On</button>";
html += "</div></div>";
html += "<script>function sendCommand(cmd) { fetch('/led?cmd=' + cmd); }</script>";
html += "</body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    String cmd = request->getParam("cmd")->value();
    if (cmd == "0") {
      digitalWrite(ledPin, LOW);
      request->send(200, "text/plain", "LED turned off");
    } else if (cmd == "1") {
      digitalWrite(ledPin, HIGH);
      request->send(200, "text/plain", "LED turned on");
    } else {
      request->send(400, "text/plain", "Invalid command");
    }
  });

  // Démarrer le serveur web
  server.begin();
}
void loop() {
}