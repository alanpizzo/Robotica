#include <WiFiNINA.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuração do OLED
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  

// Configurações de Wi-Fi
const char* ssid = "CLARO_EF05E3";
const char* password = "ZqYMJvXWT7";

WiFiServer server(80); // Servidor web na porta 80
String mensagem = "Aguardando...";  // Mensagem inicial exibida no OLED
String ipAddress = "Buscando IP...";  // Armazena o IP do Arduino

// Função para inicializar o display OLED
void iniciarDisplayOLED() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Falha ao iniciar o display OLED");
        while (true); // Loop infinito em caso de erro
    }
    atualizarDisplay(); // Atualiza a tela com valores iniciais
}

// Função para conectar-se ao Wi-Fi
void conectarWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Conectando ao WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConectado!");
    ipAddress = WiFi.localIP().toString(); // Guarda o IP
    server.begin(); // Inicia o servidor web
    atualizarDisplay(); // Exibe o IP no display OLED
}

// Função para atualizar o display OLED
void atualizarDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Exibir o IP na primeira linha
    display.setCursor(10, 10);
    display.println(ipAddress);

    // Exibir a última mensagem na segunda linha
    display.setCursor(10, 30);
    display.println(mensagem);
    
    display.display();
}

// Função para verificar conexões web e atualizar mensagens no display
void verificarConexoes() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Cliente conectado");
        String request = "";

        // Recebe requisição HTTP do navegador
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                request += c;
                if (c == '\n') break;
            }
        }

        // Verifica se há envio de mensagem via GET
        if (request.indexOf("GET /mensagem?text=") >= 0) {
            int inicio = request.indexOf("=") + 1;
            int fim = request.indexOf(" HTTP/");
            mensagem = request.substring(inicio, fim); // Extrai mensagem enviada
            mensagem.trim();
            atualizarDisplay(); // Atualiza OLED
        }

        // Página HTML para envio de mensagens
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.println("<html><body>");
        client.println("<h2>Envie uma mensagem:</h2>");
        client.println("<form action='/mensagem'>");
        client.println("<input type='text' name='text'>");
        client.println("<input type='submit' value='Enviar'>");
        client.println("</form>");
        client.println("<h3>Última mensagem: " + mensagem + "</h3>");  // Exibe a última mensagem enviada
        client.println("</body></html>");
        client.stop();
    }
}

// Configuração inicial
void setup() {
    Serial.begin(9600);
    iniciarDisplayOLED(); // Inicializa o display OLED
    conectarWiFi(); // Conecta ao Wi-Fi
}

// Loop principal
void loop() {
    verificarConexoes(); // Processa requisições web
}