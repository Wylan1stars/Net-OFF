#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h> // Biblioteca para manejar JSON (requiere instalarla desde Library Manager)

// Configuración de la red Wi-Fi
const char* ssid = "Estudiantes"; // Cambia esto a tu SSID
const char* password = "educar_2018"; // Cambia esto a tu contraseña

// Configuración del servidor y del pin del relé
ESP8266WebServer server(80);
const int ssrPin = D1; // Cambia D1 por el pin que estés usando

// Configuración del servidor remoto
const char* host = "http://tu_dominio.com/get_schedule.php?laboratorio_id=1"; // Cambia a la URL de tu servidor

// Variables para almacenar la hora de inicio y apagado
String horaInicio;
String horaApagado;

// Configuración de intervalo de tiempo para consultar la hora
unsigned long previousMillis = 0;
const long interval = 60000; // Consultar cada 60 segundos

void setup() {
  Serial.begin(115200);
  pinMode(ssrPin, OUTPUT);
  digitalWrite(ssrPin, LOW); // Asegúrate de que el relé esté apagado al inicio

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }
  Serial.print("Conectado a la red Wi-Fi. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar las rutas del servidor local
  server.on("/", handleRoot);
  server.on("/on", handleTurnOn);
  server.on("/off", handleTurnOff);

  server.begin();
  Serial.println("Servidor HTTP iniciado");

  // Obtener la hora inicial al arrancar
  obtenerHoraDelServidor();
}

void loop() {
  server.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    obtenerHoraDelServidor(); // Actualizar hora cada minuto
    verificarHora(); // Verificar si la hora actual está fuera del rango
  }
}

// Función para obtener la hora del servidor PHP
void obtenerHoraDelServidor() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(host);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      
      // Parsear el JSON recibido
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        horaInicio = doc["hora_inicio"].as<String>();
        horaApagado = doc["hora_apagado"].as<String>();
        Serial.println("Hora de inicio: " + horaInicio);
        Serial.println("Hora de apagado: " + horaApagado);
      } else {
        Serial.println("Error al parsear JSON");
      }
    } else {
      Serial.println("Error en la solicitud HTTP");
    }
    http.end();
  } else {
    Serial.println("Wi-Fi no conectado");
  }
}

// Función para verificar si la hora actual está fuera del rango y apagar el contactor
void verificarHora() {
  // Obtener la hora actual (simulada aquí)
  String horaActual = obtenerHoraActual();

  // Comparar la hora actual con el rango de horas recibido del servidor
  if (horaActual < horaInicio || horaActual >= horaApagado) {
    digitalWrite(ssrPin, LOW); // Apagar el contactor
    Serial.println("Contactor apagado, fuera de horario");
  } else {
    digitalWrite(ssrPin, HIGH); // Encender el contactor
    Serial.println("Contactor encendido, dentro del horario");
  }
}

// Función para obtener la hora actual (debes implementar esto según tu método preferido)
String obtenerHoraActual() {
  // Aquí deberías obtener la hora actual del sistema o de un servidor NTP
  // Como ejemplo, vamos a simular una hora
  return "10:30:00"; // Simula la hora actual (ajusta según tu implementación real)
}

void handleRoot() {
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; }";
  html += "h1 { color: #333; }";
  html += "button { padding: 10px 20px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; ";
  html += "background-color: #007BFF; color: white; font-size: 16px; }";
  html += "button:hover { background-color: #0056b3; }";
  html += "</style></head><body>";
  html += "<h1>Control de Contactor</h1>";
  html += "<button onclick=\"location.href='/on'\">Encender</button>";
  html += "<button onclick=\"location.href='/off'\">Apagar</button>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleTurnOn() {
  digitalWrite(ssrPin, HIGH);
  server.send(200, "text/html", "<h1>Contactor Encendido</h1><button onclick=\"location.href='/'\">Volver</button>");
  Serial.println("Contactor Encendido");
}

void handleTurnOff() {
  digitalWrite(ssrPin, LOW);
  server.send(200, "text/html", "<h1>Contactor Apagado</h1><button onclick=\"location.href='/'\">Volver</button>");
  Serial.println("Contactor Apagado");
}
