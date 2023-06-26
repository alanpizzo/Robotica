/*Bibliotecas
===================================================================================================================*/
#include <SPI.h>
#include <Wire.h> // define as portas do display A4(SDA), A5(SCL)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h> //biblioteca do teclado matricial


/*Variáveis e definições
===================================================================================================================*/
char comandoCentral=Serial.read();

char* password = "4321"; /* Define a senha para destravar a fechadura.               */
int caracteres = 4; /* Quantidade de caracteres que a senha possui.
*/
//#define pinoLedWifi 23 //pino do led que informa sobre a autorização ou não da central de segurança
#define pinoBuzzer 11//pino do buzzer
#define pinoFechadura 10 //pino da fechadura
int posicao = 0; //controle da posição do servo
                                             
#define qtdLinhas 4 //define o número de linhas do teclado matricial de membrana
#define qtdColunas 4 //define o número de linhas do teclado matricial de membrana

char matriz_teclas[qtdLinhas][qtdColunas] = {/* Construção da matriz de caracteres.                      */

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte PinosqtdLinhas[qtdLinhas] = {9, 8, 7, 6}; /* Define os pinos de controle de linhas e colunas.         */
byte PinosqtdColunas[qtdColunas] = {5, 4, 3, 2};/* Define os pinos de controle de linhas e colunas.         */

/*Objetos
===================================================================================================================*/
Adafruit_SSD1306 display(128, 64, &Wire, -1); //Display OLED 128X64
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas);


void setup() {
  pinMode(pinoBuzzer,OUTPUT);
  pinMode(pinoFechadura,OUTPUT);
//  pinMode(pinoLedWifi,OUTPUT);

  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicia o diplay OLED
  limpaTela(); //chama a função para limpar as informações do display


  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(13,15); //posicao do cursor no display           
  display.println("ROBOTICA"); //mensagem de texo a ser impressa na tela
  display.setCursor(15,45); //posicao do cursor no display           
  display.println("2CPM LDA"); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(2500);
  limpaTela();
   
  trancada();  /* Inicia com a fechadura trancada */

}
void loop() {
  Serial.println(comandoCentral);
  char comandoCentral = Serial.read(); 
  /* Armazena na variável key a tecla pressionada.          */
  char key = meuteclado.getKey();
  /* Se a tecla pressionada for "*" ou "#" reinicia a       */
  /* tentativa com a fechadura trancada.                    */
  if(key != NO_KEY){
  tone(pinoBuzzer,195,120);
  }
  if (key == '*' || key == '#' || key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    posicao = 0;
    trancada();
    
  }
  /* Se as teclas pressionadas coincidirem com a senha,     */
  /* destranque a fechadura.                                */
  if (key == password[posicao]) {
    posicao ++;
  }
  if ((posicao == caracteres)&&(comandoCentral=='A')) {
    destrancada();
  }
  /* Pequena pausa para retomar a leitura.                  */
  delay(100);
}
/* Função que mantém a fechadura trancada.                  */
void trancada()
{
  /* LED Vermelho acende.                                   */
  /* LED Verde apaga.                                       */
  /* Servo na posição trancada.                             */
  digitalWrite(pinoFechadura,LOW);
  /* Imprime a palavra no Monitor Serial.                   */
 
  testfillroundrect();
  delay(500);
  display.clearDisplay();
  
  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(5,5); //posicao do cursor no display           
  display.println("Digite sua senha:"); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(500);

  tone(pinoBuzzer,195,250);

  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_BLACK,WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(0,45); //posicao do cursor no display           
  display.println(" TRANCADA  "); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(1000);
  display.clearDisplay();

}
/* Função que mantém a fechadura destrancada.               */
void destrancada()
{
  /* LED Verde acende.                                      */
  /* LED Vermelho apaga.                                    */
  /* Servo na posição destrancada.                          */
  digitalWrite(pinoFechadura,HIGH);
  testfillroundrect(); // Draw rounded rectangles (outlines)
  delay(500);
  display.clearDisplay();

  tone(pinoBuzzer,195,200);
  delay(250);
  tone(pinoBuzzer,195,200);

  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_BLACK,WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(0,45); //posicao do cursor no display           
  display.println("  ABERTA  "); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(1000);
}
void limpaTela(){//limpa a tela do display
  display.clearDisplay();
  display.display();  
}

void testfillroundrect(void) {//efeito retangulos arredondados
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}
