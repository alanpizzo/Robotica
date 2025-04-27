#include <WiFiNINA.h>
#include <ArduinoOTA.h>

// Configurações de WiFi
const char* ssid = "CLARO_EF05E3";
const char* password = "ZqYMJvXWT7";

void setup() {
  Serial.begin(9600);
  while (!Serial); // Aguarda a porta serial estar pronta (apenas para debug)

  // Conecta ao WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado! Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configuração OTA
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
  
  Serial.println("Pronto para atualização OTA");
  Serial.println("Endereço IP: " + WiFi.localIP().toString());
}

void loop() {
  // Verifica constantemente por atualizações OTA
  ArduinoOTA.poll();
  
  // Seu código normal aqui...
  // ...
}