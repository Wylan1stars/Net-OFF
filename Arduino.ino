#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Estudiantes";
const char* password = "educar_2018";
const char* serverName = "http://db.tecnica4.edu.ar/check_time.php"; // URL del script PHP

const int ssrPin = D1; // Pin del contactor

void setup() {
  Serial.begin(115200);
  pinMode(ssrPin, OUTPUT);
  digitalWrite(ssrPin, HIGH); // Inicialmente, contactor encendido

  // Conexión a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Conectando a Wi-Fi...");
  }
  Serial.println("Conectado a Wi-Fi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName); // Iniciar conexión HTTP

      int httpResponseCode = http.GET(); // Solicitud GET
      if (httpResponseCode > 0) {
          String payload = http.getString();
          Serial.println(payload); // Ver la respuesta del servidor

          // Parsear el JSON
          StaticJsonDocument<200> doc;
          DeserializationError error = deserializeJson(doc, payload);

          if (!error) {
              bool apagado = doc["apagado"];
              if (apagado) {
                  digitalWrite(ssrPin, LOW); // Apagar el contactor
              } else {
                  digitalWrite(ssrPin, HIGH); // Encender el contactor
              }
          } else {
              Serial.println("Error al analizar JSON");
          }
      } else {
          Serial.print("Error en la petición HTTP: ");
          Serial.println(httpResponseCode);
      }

      http.end(); // Terminar conexión
  } else {
      Serial.println("Error de conexión Wi-Fi");
  }

  delay(30000); // Consultar cada 30 segundos
}
