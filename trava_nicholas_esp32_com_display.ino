#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>
#include <ArduinoOTA.h>

// Definições de hardware e conexão física
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_SDA 4
#define OLED_SCL 5

#define SS_PIN 21   
#define RST_PIN 22  
#define LOCK_PIN 25 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MFRC522 rfid(SS_PIN, RST_PIN);
WiFiClient espClient;
PubSubClient client(espClient);

const char *ssid = "CLARO_EF05E3";
const char *password = "ZqYMJvXWT7";
const char *mqtt_server = "broker.hivemq.com";

const String validTags[] = {"99 05 80 72", "E7 31 77 53", "60 2F B8 89"};

// Declaração antecipada das funções para evitar erro de escopo
void setupPins();
void setupDisplay();
void setupRFID();
void connectWiFi();
void connectMQTT();
void setupOTA();
void displayMessage(String msg);
String readTag();
void processTag(String tagID);
void mqttCallback(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(115200);

  setupPins();
  setupDisplay();
  setupRFID();
  connectWiFi();
  connectMQTT();
  setupOTA();

  displayMessage("Aproxime a TAG");
}

void loop() {
  client.loop();
  ArduinoOTA.handle();

  while (true) {
    client.loop();
    ArduinoOTA.handle();
    
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      String tagID = readTag();
      processTag(tagID);
      rfid.PICC_HaltA();
    }

    delay(100);
  }
}

void setupPins() {
  pinMode(LOCK_PIN, OUTPUT);
  digitalWrite(LOCK_PIN, LOW);
}

void setupDisplay() {
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void setupRFID() {
  SPI.begin();
  rfid.PCD_Init();
}

void connectWiFi() {
  displayMessage("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi conectado!");
  displayMessage("WiFi OK\nIP: " + WiFi.localIP().toString());
}

void connectMQTT() {
  displayMessage("Conectando ao MQTT...");
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);

  while (!client.connected()) {
    if (client.connect("ESP32_Client")) {
      Serial.println("Conectado ao MQTT!");
      displayMessage("MQTT OK");
      client.subscribe("trava/liberacao");
    } else {
      Serial.print("Falha na conexão MQTT. Tentando novamente...");
      delay(5000);
    }
  }
}

void setupOTA() {
  ArduinoOTA.begin();
}

void displayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(msg);
  display.display();
}

String readTag() {
  String tagID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    tagID += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) tagID += " ";
  }
  
  Serial.println("Tag lida: " + tagID);
  return tagID;
}

void processTag(String tagID) {
  bool authorized = false;
  
  for (String validTag : validTags) {
    if (tagID.equalsIgnoreCase(validTag)) {
      authorized = true;
      break;
    }
  }

  if (!authorized) {
    displayMessage("Não autorizado");
    client.publish("trava/solicitacao", "tag invalida");
    delay(2000);
    return;
  }

  client.publish("trava/solicitacao", "solicitacao");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  if (String(topic) == "trava/liberacao") {
    if (message == "liberar") {
      displayMessage("Liberado");
      digitalWrite(LOCK_PIN, HIGH);
      delay(180000);
      digitalWrite(LOCK_PIN, LOW);
    } else if (message == "negar") {
      displayMessage("Não autorizado");
      delay(2000);
    }
  }
}
