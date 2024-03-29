/*==================================================================================================
 * 
 *                  R  O  B  Ó  T  I  C  A     2°  C  P  M
 *                  
 *                        Cofre Wifi - RFID - ESP32
 *                        
 *                     Código - versão 4.1 - Março 2023.
 *                     
 *                     Orientador: Alan Machado Pizzo
 *                     
 *                     Alunos: Nathan Câmara, Felipe Gonçalves Araújo e Nicholas Goes
 *                     
 *====================================================================================================
  Bibliotecas
  ====================================================================================================*/
#include <SPI.h>      //biblioteca protocolo SPI
#include <MFRC522.h>  //biblioteca leitor RFID
#include <WiFi.h>     //biblioteca para controlar conexão Wifi

/* Variáveis e definições
  ====================================================================================================*/
#define PINO_SDA 21         //pino SDA do leitor RFID
#define PINO_RST 22         //pino RST do leitor RFID
#define pinoLedvermelho 32  //pino do led vermelho
#define pinoLedverde 12     //pino do led verde
#define pinoTrava 33        //pino do solenóide
#define pinLedazul 5        //pino led azul 
const char* ssid     = "digitar o nome da rede entre aspas";
const char* password = "digitar senha da rede wifi entre aspas";

/*Instanciação de objetos
  ===================================================================================================*/
MFRC522 mfrc522(PINO_SDA, PINO_RST);  //Cria objeto para leitor RFID
WiFiServer server(80);                //Cria o servidor Wifi na porta 80

/*Protótipo da função
  ====================================================================================================*/
void rfid_func();  //Identificação de tags RFID

void setup()
{
    /*Modo de operação dos pinos
    ====================================================================================================*/
  pinMode(pinoLedverde, OUTPUT);     //pino do led verde
  pinMode(pinoLedvermelho, OUTPUT);  //pino do led vermelho
  pinMode(pinoTrava, OUTPUT);        //saída para trava eletrônica
  pinMode(pinLedazul, OUTPUT);      // pino do led azul
  
 /*Inicialização da comunicação serial e leitor RFID
    ====================================================================================================*/
  Serial.begin(9600);  //inicializa Serial
  SPI.begin();         //inicializa SPI
  mfrc522.PCD_Init();  //inicializa o leitor RFID

    delay(10);

/*Conexão da rede Wifi
========================================================================================================*/
    Serial.println();
    Serial.println();
    Serial.print("Conectando...");
    Serial.println(ssid);
 //Imprime mensagem inicial
  Serial.println("Cofre Wifi - RFID - 2CPM");
  Serial.println("versao 4.1           marco   2023");
  Serial.println("Aproxime a tag do leitor...");
  Serial.println();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado.");
    Serial.println("Endereço IP: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

void loop(){
 WiFiClient client = server.available();   // Aguardando a conexão de novo clientes

  if (client) {                             // se um cliente se conectar
    Serial.println("New Client.");          // imprime a mensagem na porta serial
    String currentLine = "";                // armazena dados co cliente em uma String
    while (client.connected()) {            // laço enquanto o cliente está conectado
      if (client.available()) {             // se há bytes para ler do cliente
        char c = client.read();             // lê um byte, então
        Serial.write(c);                    // imprime o dado no monitor serial
        if (c == '\n') {                    // se o byte é um caracter de nova linha

          // se a linha atual for vazia, temos dois novos caracteres de nova linha na sequência.
          // este é o fim da requisição ao HTTP client, então a mensagem é enviada:
          if (currentLine.length() == 0) {
            // Cabeçalhos HTTP sempre começam com o código de resposta (e.g. HTTP/1.1 200 OK)
            // e o tipo do conteúdo, então o cliente saberá o que estará recebendo, então uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // O conte´dudo da resposta http segue o cabeçalho:
            client.print("Cofre Wifi - RFID - 2 Colegio da Policia Militar do Parana");
            client.print("Click <a href=\"/H\">here</a> Autorizar abertura do cofre.<br>");
            client.print("Click <a href=\"/L\">here</a> Bloquear abertura do cofre.<br>");

            // A resposta HTTP termina com outra linha em branco:
            client.println();
            // Fim do laço enquanto:
            break;
          } else {    // se obter uma nova linha, então limpe a linha atual:
            currentLine = "";
          }
        } else if (c != '\r') {  // se obter um caracter de transporte,
          currentLine += c;      // adiciona-lo ao final da linha
        }

        // Checar se a requisição do cliente foi "GET /H" ou "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(pinLedazul, HIGH);               // GET /H acende o LED azul e habilita a abertura local do cofre
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(pinLedazul, LOW);                // GET /L apaga o LED azul e desabilita o acesso local ao cofre
        }
      }
    }
    // Fecha a conexão:
    client.stop();
    Serial.println("Cliente desconectado.");
  }
  rfid_func();  //chama função para a idenficação de tags
}
/*Funções
  ====================================================================================================*/
void rfid_func()  //Função para identificação de tags RFID
{
  // -- Verifica novas tags --
  if (!mfrc522.PICC_IsNewCardPresent()) return;

  // Seleciona uma das tags
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Mostra a identificação da tag na serial
  Serial.print("UID da tag :");
  String conteudo = "";
  byte letra;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();


  //tags autorizadas a acessar a fechadura(inclui tags autorizadas e permissão remota da central de controle)
  if (digitalRead(pinLedazul)==HIGH&&(conteudo.substring(1) == "99 05 80 72" || conteudo.substring(1) == "E7 31 77 53" || conteudo.substring(1) == "60 2F B8 89")) {
    //Mensagem para acesso negado
    Serial.println("Autorizado!");
    Serial.println();

    //led verde para acesso autorizado
    for (int i = 0; i < 6; i++) {
      digitalWrite(pinoLedvermelho, LOW);  //desliga pinoBuzzer
      digitalWrite(pinoLedverde, HIGH);    //end else
      delay(100);
      digitalWrite(pinoLedverde, LOW);
      delay(100);
    }

    //Liberação da trava
    digitalWrite(pinoTrava, HIGH);
    delay(5000);
    digitalWrite(pinoTrava, LOW);
  }  //end if

  //tags sem acesso autorizado
  else {
    //mensagem para tags não autorizadas
    Serial.println("Nao autorizado");
    Serial.println();

    // Led vermelho para acesso negado
    for (int i = 0; i < 6; i++) {
      digitalWrite(pinoLedvermelho, HIGH);  //pisca o pino vemelho
      digitalWrite(pinoLedverde, LOW);      //mantem o pino verde apagado
      delay(100);
      digitalWrite(pinoLedvermelho, LOW);
      delay(100);
    }
  }  // fim do else
}  //fim da função
