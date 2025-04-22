#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// **Configuração da tela OLED 128x64 da Adafruit**
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset não necessário na maioria dos módulos
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// **Conexão do display OLED ao Arduino MKR 1010 WiFi**
/*
O display OLED deve ser conectado via **I2C** nos seguintes pinos:
- **SDA** → Pino **SDA** do Arduino MKR 1010
- **SCL** → Pino **SCL** do Arduino MKR 1010
- **VCC** → 3.3V do Arduino MKR 1010 (não use 5V)
- **GND** → GND do Arduino MKR 1010

O MKR 1010 já possui comunicação I2C nos pinos padrão, por isso a biblioteca `Wire.h` gerencia a conexão automaticamente.
*/

// **Configuração da rede WiFi**
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
const int relePin = 5;  // Pino de saída para ativar a trava
const int rfidSSPin = 10;  // Pino SDA (SS) do módulo RFID
const int rfidRSTPin = 9;  // Pino RST do módulo RFID
MFRC522 rfid(rfidSSPin, rfidRSTPin);

bool aguardandoLiberacao = false;  // Estado da solicitação
unsigned long tempoSolicitacao = 0;  // Tempo do último acesso

// **Lista de tags RFID válidas**
String tagsValidas[] = {"99 05 80 72", "E7 31 77 53", "60 2F B8 89"};

// **Função para exibir mensagens no display OLED**
void exibirMensagem(const char* mensagem) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 25);
    display.print(mensagem);
    display.display();
}

// **Função para conectar ao WiFi**
void conectarWiFi() {
    Serial.print("Conectando ao WiFi...");
    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Conectado ao WiFi!");
    exibirMensagem("WiFi conectado");
}

// **Função para conectar ao servidor MQTT**
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
    exibirMensagem("MQTT conectado");
}

// **Função para verificar mensagens recebidas via MQTT**
void verificarMensagensMQTT() {
    if (mqttClient.parseMessage()) {
        String mensagem = mqttClient.readString();
        Serial.print("Mensagem recebida: ");
        Serial.println(mensagem);

        if (mensagem == "liberar" && aguardandoLiberacao) {
            digitalWrite(relePin, HIGH);
            Serial.println("Trava aberta!");
            exibirMensagem("Acesso liberado!");
            aguardandoLiberacao = false;
        }
    }
}

// **Função para verificar se a tag RFID é válida**
bool tagValida(String tag) {
    for (String validTag : tagsValidas) {
        if (validTag == tag) {
            return true;
        }
    }
    return false;
}

// **Função para verificar a leitura da tag RFID e solicitar acesso via MQTT**
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
        exibirMensagem("Aguardando autorizacao");
        mqttClient.beginMessage(mqttTopicRequest);
        mqttClient.print("solicitacao");
        mqttClient.endMessage();

        aguardandoLiberacao = true;
        tempoSolicitacao = millis();
    } else {
        Serial.println("Acesso negado. Tag inválida.");
        exibirMensagem("Tag invalida!");
    }
}

// **Função para inicializar os dispositivos**
void inicializarDispositivos() {
    Serial.begin(115200);
    pinMode(relePin, OUTPUT);
    digitalWrite(relePin, LOW);
  
    SPI.begin();
    rfid.PCD_Init();

    // **Inicialização do Display OLED**
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Endereço I2C padrão
        Serial.println("Falha ao iniciar o display OLED!");
        while (true);
    }
    display.setTextSize(2);
    display.setTextColor(WHITE);
    exibirMensagem("Inicializando...");

    conectarWiFi();
    conectarMQTT();
}

// **Loop principal**
void loop() {
    if (!mqttClient.connected()) {
        conectarMQTT();
    }

    verificarMensagensMQTT();
    verificarRFID();

    if (aguardandoLiberacao && (millis() - tempoSolicitacao > 60000)) {
        Serial.println("Tempo expirado. Use a tag novamente.");
        exibirMensagem("Tempo expirado!");
        aguardandoLiberacao = false;
    }
}

// **Setup inicial**
void setup() {
    inicializarDispositivos();
}