#include <WiFiNINA.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuração do OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuração da rede WiFi
char ssid[] = "CLARO_EF05E3";     
char pass[] = "ZqYMJvXWT7";
WiFiServer server(80);
WiFiClient client;

// Variáveis do cronômetro e placar
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool running = false;
bool paused = false;
bool blinkState = false;
unsigned long lastBlinkTime = 0;

// Variáveis de pontuação
int scoreTime1 = 0;
int scoreTime2 = 0;

/**
 * Inicializa a conexão WiFi e exibe o IP atribuído.
 */
void iniciarWiFi() {
    WiFi.begin(ssid, pass);

    Serial.print("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado!");
    Serial.print("IP atribuído: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

/**
 * Inicializa o display OLED.
 */
void iniciarDisplay() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Falha ao inicializar OLED!");
        while (true);
    }
    display.clearDisplay();
}

/**
 * Processa requisições HTTP dos botões na página.
 */
void processarRequisicoes() {
    client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        if (request.indexOf("/start") != -1) {
            startTime = millis() - elapsedTime;
            running = true;
            paused = false;
        } else if (request.indexOf("/pause") != -1) {
            running = false;
            paused = true;
            elapsedTime = millis() - startTime;
        } else if (request.indexOf("/reset") != -1) {
            running = false;
            paused = false;
            elapsedTime = 0;
        } else if (request.indexOf("/score1+") != -1) {
            scoreTime1++;
        } else if (request.indexOf("/score1-") != -1) {
            if (scoreTime1 > 0) scoreTime1--;
        } else if (request.indexOf("/score2+") != -1) {
            scoreTime2++;
        } else if (request.indexOf("/score2-") != -1) {
            if (scoreTime2 > 0) scoreTime2--;
        }

        // Gera resposta da página HTML com estilo
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<html><head><style>");
        client.println("body { font-family: Arial; text-align: center; background: #f4f4f4; }");
        client.println("button { font-size: 20px; padding: 10px; margin: 5px; background: #007bff; color: white; border: none; cursor: pointer; }");
        client.println("button:hover { background: #0056b3; }");
        client.println("h1 { color: #333; } h2 { color: #555; }");
        client.println("</style></head><body>");
        client.println("<h1>Cronômetro e Placar</h1>");
        client.println("<button onclick=\"location.href='/start'\">Iniciar</button>");
        client.println("<button onclick=\"location.href='/pause'\">Pausar</button>");
        client.println("<button onclick=\"location.href='/reset'\">Zerar</button>");
        client.println("<h2>Placar</h2>");
        client.println("<p>Casa: <b>" + String(scoreTime1) + "</b> <button onclick=\"location.href='/score1+'\">+</button> <button onclick=\"location.href='/score1-'\">-</button></p>");
        client.println("<p>Vis: <b>" + String(scoreTime2) + "</b> <button onclick=\"location.href='/score2+'\">+</button> <button onclick=\"location.href='/score2-'\">-</button></p>");
        client.println("</body></html>");
        client.stop();
    }
}

/**
 * Formata o tempo em string no formato 00:00:00 (minutos, segundos e centésimos).
 */
String formatarTempo(unsigned long tempoAtual) {
    int minutos = (tempoAtual / 60000) % 60;
    int segundos = (tempoAtual / 1000) % 60;
    int centesimos = (tempoAtual / 10) % 100;

    String tempoStr = String(minutos) + ":" +
                      (segundos < 10 ? "0" : "") + String(segundos) + ":" +
                      (centesimos < 10 ? "0" : "") + String(centesimos);
    return tempoStr;
}

/**
 * Atualiza o display OLED com o cronômetro centralizado e o placar.
 */
void atualizarDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("IP: ");
    display.println(WiFi.localIP());

    unsigned long tempoAtual = running ? millis() - startTime : elapsedTime;
    String tempoStr = formatarTempo(tempoAtual);

    // Centraliza o cronômetro na tela
    display.setTextSize(2);
    int textWidth = 12 * tempoStr.length();
    int xOffset = (SCREEN_WIDTH - textWidth) / 2;
    int yOffset = 15;

    // Pisca quando pausado
    if (paused) {
        if (millis() - lastBlinkTime > 500) {
            blinkState = !blinkState;
            lastBlinkTime = millis();
        }
        if (blinkState) {
            display.setCursor(xOffset, yOffset);
            display.print(tempoStr);
        }
    } else {
        display.setCursor(xOffset, yOffset);
        display.print(tempoStr);
    }

    // Exibir separação "Casa     Vis"
    display.setTextSize(1);
    display.setCursor(14, 35);
    display.print("CASA     VISITANTE");

    // Exibição do placar no formato "00 X 00"
    display.setTextSize(2);
    String placarStr = (scoreTime1 < 10 ? "0" : "") + String(scoreTime1) +
                       " X " + (scoreTime2 < 10 ? "0" : "") + String(scoreTime2);

    int placarWidth = 12 * placarStr.length();
    int placarOffset = (SCREEN_WIDTH - placarWidth) / 2;

    display.setCursor(placarOffset, 50);
    display.print(placarStr);

    display.display();
}

void setup() {
    Serial.begin(115200);
    iniciarWiFi();
    iniciarDisplay();
}

void loop() {
    processarRequisicoes();
    atualizarDisplay();
    delay(100);
}