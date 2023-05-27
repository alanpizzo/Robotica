#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "ESPCofre"
#define APPSK "CAPIVARIAS"
#endif

String webPage="";

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", webPage);
}

void setup() {
  webPage += "<h1>Capivarias Security</h1><p><a href=\"ABRE\"><button>ABRIR</button></a>";
  webPage += "<p><a href=\"FECHA\"><button>FECHAR</button></a>";
  
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectando ao cofre...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Endereco IP: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  server.on("/ABRE", [](){
      Serial.write('A');
  });
server.on("/FECHA", [](){
      Serial.write('B');
  });
}
void loop() {
  server.handleClient();
}
