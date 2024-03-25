/*==================================================================================================
  |
  |                           2   C  P  M
  |
  |             F E C H A D U R A   E L E T R Ô N I C A     R F I D
  ====================================================================================================

  Bibliotecas
  ====================================================================================================*/
#include <SPI.h>      //biblioteca protocolo SPI
#include <MFRC522.h>  //biblioteca leitor RFID

/* Variáveis e definições
  ====================================================================================================*/
#define PINO_SDA 21         //pino SDA do leitor RFID
#define PINO_RST 22         //pino RST do leitor RFID
#define pinoLedvermelho 32  //pino do led vermelho
#define pinoLedverde 12     //pino do led verde
#define pinoTrava 33        //pino do solenóide


/*Instanciação de objetos
  ===================================================================================================*/
MFRC522 mfrc522(PINO_SDA, PINO_RST);  //Cria objeto para leitor RFID

/*Protótipo da função
  ====================================================================================================*/
void rfid_func();  //Identificação de tags RFID


void setup() {
  /*Modo de operação dos pinos
    ====================================================================================================*/
  pinMode(pinoLedverde, OUTPUT);     //pino do led verde
  pinMode(pinoLedvermelho, OUTPUT);  //pino do led vermelho
  pinMode(pinoTrava, OUTPUT);        //saída para trava eletrônica

  /*Inicialização dos leds e da trava
    ====================================================================================================*/
  digitalWrite(pinoLedvermelho, LOW);  //desliga led vermelho
  digitalWrite(pinoLedverde, LOW);     //desliga led verde
  digitalWrite(pinoTrava, LOW);        //desliga trava

  /*Inicialização da comunicação serial e leitor RFID
    ====================================================================================================*/
  Serial.begin(9600);  //inicializa Serial
  SPI.begin();         //inicializa SPI
  mfrc522.PCD_Init();  //inicializa o leitor RFID

  //Imprime mensagem inicial
  Serial.println("Fechadura Eletronica RFID/wifi 2CPM");
  Serial.println("v. beta           marco   2023");
  Serial.println("Aproxime a tag do leitor...");
  Serial.println();
}

void loop() {
  //
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

  // Mostra UID na serial
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


  //tags autorizadas a acessar a fechadura
  if (conteudo.substring(1) == "99 05 80 72" || conteudo.substring(1) == "E7 31 77 53" || conteudo.substring(1) == "60 2F B8 89") {
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
      digitalWrite(pinoLedvermelho, HIGH);  //desliga pinoBuzzer
      digitalWrite(pinoLedverde, LOW);      //end else
      delay(100);
      digitalWrite(pinoLedvermelho, LOW);
      delay(100);
    }
  }  // fim do else
}  //fim da função
