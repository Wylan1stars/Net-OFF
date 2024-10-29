#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configuración de la red Wi-Fi
const char* ssid = "Estudiantes"; // Cambia esto a tu SSID
const char* password = "educar_2018"; // Cambia esto a tu contraseña

// Crear un objeto del servidor en el puerto 80
ESP8266WebServer server(80);

// Definir el pin del relé (o contactor)
const int ssrPin = D1; // Cambia D1 por el pin que estés usando

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
  Serial.println(WiFi.localIP()); // Mostrar dirección IP

  // Configurar las rutas del servidor
  server.on("/", handleRoot); // Página principal
  server.on("/on", handleTurnOn); // Ruta para encender el contactor
  server.on("/off", handleTurnOff); // Ruta para apagar el contactor

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient(); // Manejar las solicitudes del cliente
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
  
  server.send(200, "text/html", html); // Enviar HTML al cliente
}

void handleTurnOn() {
  digitalWrite(ssrPin, HIGH); // Encender el contactor
  server.send(200, "text/html", "<h1>Contactor Encendido</h1><button onclick=\"location.href='/'\">Volver</button>");
  Serial.println("Contactor Encendido");
}

void handleTurnOff() {
  digitalWrite(ssrPin, LOW); // Apagar el contactor
  server.send(200, "text/html", "<h1>Contactor Apagado</h1><button onclick=\"location.href='/'\">Volver</button>");
  Serial.println("Contactor Apagado");
}
