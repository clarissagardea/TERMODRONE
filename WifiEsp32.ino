#include <WiFi.h>

const char* ssid = "T";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Reiniciando WiFi...");
  WiFi.disconnect(true); // Limpia configuraciones previas
  delay(1000);
  WiFi.mode(WIFI_STA); // Modo cliente

  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) { // 30 segundos
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConexión WiFi exitosa!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNo se pudo conectar al WiFi.");
    Serial.println("Revisa las credenciales o la configuración de tu red.");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Intentando reconectar...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    delay(10000); // Espera antes de intentar de nuevo
  } else {
    Serial.println("Conexión estable.");
  }
  delay(5000);
}


/*
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Asegúrate de que no está conectado previamente

  delay(1000);
  Serial.println("Escaneando redes disponibles...");

  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No se encontraron redes.");
  } else {
    Serial.println("Redes encontradas:");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i)); // Fuerza de la señal
      Serial.println(" dBm)");
      delay(10);
    }
  }
}

void loop() {
  // Nada que hacer aquí
}
*/
