#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

// Definir el pin del sensor de corriente y el relé
const int currentSensorPin = A0;
const int ssrPin = D1; // Pin para el contactor de estado sólido
const int relayPin = D2; // Pin para el relé de la térmica

// Configuración de la red Wi-Fi
const char* ssid = "Estudiantes";
const char* password = "educar_2018";

AsyncWebServer server(80);

float readCurrent() {
  int sensorValue = analogRead(currentSensorPin);
  // Conversión básica para obtener la corriente (debes calibrar esto)
  float voltage = sensorValue * (3.3 / 1023.0);
  float current = (voltage - 2.5) / 0.066; // Ajusta esto para tu sensor específico
  return current;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ssrPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(ssrPin, LOW); // Inicialmente apagado
  digitalWrite(relayPin, HIGH); // Térmica encendida por defecto
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }
  Serial.println("Conectado a Wi-Fi");

  // Configuración del servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    float current = readCurrent();
    String html = "<h1>Monitoreo de corriente</h1>";
    html += "<p>Corriente actual: " + String(current) + " A</p>";
    html += "<a href=\"/turnon\">Encender</a> | <a href=\"/turnoff\">Apagar</a>";
    request->send(200, "text/html", html);
  });

  server.on("/turnon", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ssrPin, HIGH); // Encender el contactor
    digitalWrite(relayPin, HIGH); // Asegurar que la térmica esté encendida
    request->send(200, "text/plain", "Contactor encendido");
  });

  server.on("/turnoff", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ssrPin, LOW); // Apagar el contactor
    digitalWrite(relayPin, LOW); // Apagar la térmica
    request->send(200, "text/plain", "Contactor apagado");
  });

  server.begin();
}

void loop() {
  // Aquí podrías agregar alguna lógica adicional de monitoreo si es necesario
}
