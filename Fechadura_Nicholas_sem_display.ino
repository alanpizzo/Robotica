#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <MFRC522.h>
#include <SPI.h>

// **Configurações da rede WiFi**
const char* ssid = "2CPM_ROBOTICA";  
const char* password = "#@robotica1";  

// **Configuração do servidor MQTT**
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopicRequest = "trava/solicitacao";  
const char* mqttTopicResponse = "trava/liberacao";  

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// **Pinos do relé e leitor RFID**
const int relePin = 5;  // Pino de saída para o relé

// **Conexão do leitor RFID MFRC522**
const int rfidSSPin = 10;  // Pino SDA (SS) do módulo RFID
const int rfidRSTPin = 9;  // Pino RST do módulo RFID
MFRC522 rfid(rfidSSPin, rfidRSTPin);

bool aguardandoLiberacao = false;
unsigned long tempoSolicitacao = 0;

// **Lista de tags RFID válidas**
String tagsValidas[] = {"99 05 80 72", "E7 31 77 53", "60 2F B8 89"};

// **Conectando o leitor RFID MFRC522 ao Arduino MKR 1010 WiFi**
/*
Conecte o módulo RFID MFRC522 aos pinos do Arduino MKR 1010 WiFi conforme abaixo:
- **SDA (SS)** → Pino **10** (definido como `rfidSSPin`)
- **SCK** → Pino **13** (padrão SPI)
- **MOSI** → Pino **11** (padrão SPI)
- **MISO** → Pino **12** (padrão SPI)
- **RST** → Pino **9** (definido como `rfidRSTPin`)
- **GND** → GND do Arduino
- **VCC** → 3.3V do Arduino (o MKR 1010 opera com 3.3V!)
*/

void conectarWiFi() {
    Serial.print("Conectando ao WiFi...");
    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Conectado ao WiFi!");
}

void conectarMQTT() {
    Serial.print("Conectando ao broker MQTT...");

    mqttClient.setId("ArduinoMKR1010");
    mqttClient.setUsernamePassword("", "");  

    while (!mqttClient.connect(mqttServer, mqttPort)) {
        Serial.print(".");
        delay(2000);
    }

    Serial.println("Conectado ao MQTT!");
    mqttClient.subscribe(mqttTopicResponse);
}

void verificarMensagensMQTT() {
    if (mqttClient.parseMessage()) {
        String mensagem = mqttClient.readString();
        Serial.print("Mensagem recebida: ");
        Serial.println(mensagem);

        if (mensagem == "liberar" && aguardandoLiberacao) {
            digitalWrite(relePin, HIGH);
            Serial.println("Trava aberta!");
            aguardandoLiberacao = false;
        }
    }
}

// **Verifica se a tag RFID é válida**
bool tagValida(String tag) {
    for (String validTag : tagsValidas) {
        if (validTag == tag) {
            return true;
        }
    }
    return false;
}

// **Lê a tag RFID e solicita acesso via MQTT**
void verificarRFID() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    // **Obtém o ID da tag RFID**
    String tagID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        tagID += String(rfid.uid.uidByte[i], HEX);
        if (i < rfid.uid.size - 1) {
            tagID += " ";
        }
    }

    Serial.print("Tag detectada: ");
    Serial.println(tagID);

    if (tagValida(tagID)) {
        Serial.println("Tag autorizada. Enviando solicitação de acesso...");
        mqttClient.beginMessage(mqttTopicRequest);
        mqttClient.print("solicitacao");
        mqttClient.endMessage();

        aguardandoLiberacao = true;
        tempoSolicitacao = millis();
    } else {
        Serial.println("Acesso negado. Tag inválida.");
    }
}

// **Inicializa os dispositivos**
void inicializarDispositivos() {
    Serial.begin(115200);
    pinMode(relePin, OUTPUT);
    digitalWrite(relePin, LOW);
  
    SPI.begin();  // **Inicializa a comunicação SPI**
    rfid.PCD_Init();  // **Inicializa o módulo RFID**

    conectarWiFi();
    conectarMQTT();
}

void loop() {
    if (!mqttClient.connected()) {
        conectarMQTT();
    }

    verificarMensagensMQTT();
    verificarRFID();

    if (aguardandoLiberacao && (millis() - tempoSolicitacao > 60000)) {
        Serial.println("Tempo expirado. Use a tag novamente.");
        aguardandoLiberacao = false;
    }
}

void setup() {
    inicializarDispositivos();
}