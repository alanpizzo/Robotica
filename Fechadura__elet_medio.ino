/*=================================================================================================
 *                     R O B O T I C A - 2 C P M  - L O N D R I N A
===================================================================================================*/
/*Bibliotecas
=================================================================================================== */
#include <SPI.h>
#include <Wire.h> // define as portas do display A4(SDA), A5(SCL)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#define linhas 4
#define colunas 3
#define led_vermelho 12
#define led_verde 13
#define led_amarelo 10
#define pinoBuzzer 11

char mapaTeclas[4][4]={//4 linhas e quatro colunas
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
  };
String senha="1234";
String digitada;
int estado=0;
int aberto=0;
byte pinos_linha[4]={9,8,7,6};
byte pinos_colunas[4]={5,4,3,2};

Keypad teclado = Keypad( makeKeymap(mapaTeclas),pinos_linha,pinos_colunas,4,4);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicia o diplay OLED
  display.clearDisplay(); //chama a função 

  pinMode(pinoBuzzer,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);

  telaInicial();
  msgEspera();
    }
    
    
    void loop() {
    
    char tecla=teclado.getKey();
      if(tecla !=NO_KEY){
        estado=1;
        tone(pinoBuzzer,240,150);
        if(tecla=='#'){
          if(verificaSenha(senha,digitada)){
            estado=3;
            leds(estado);
            aberto=1;
          }else{
            estado=2;
            leds(estado);
            delay(3000);
            estado=0;
          }
          digitada="";
        }else{
          digitada+=tecla;
        }
        leds(estado);
      }
    if(estado==3 && aberto==1){
      if(tecla != NO_KEY){
      estado=0;
      aberto=0;
      digitada="";
      }
   }
    Serial.println(senha);
    Serial.println(digitada);
   }
bool verificaSenha(String sa, String sd) {
  bool resultado=false;
  if(sa.compareTo(sd)==0){
    resultado=true;
  }else{
    resultado=false;
  }
     return resultado;
     }
     
     void leds(int e){//0=Aguardando / 1=Digitando / 2=Negado / 3=Aceito
       if(e==0){
         msgEspera();
       }else if(e==1){
         digitalWrite(led_vermelho,LOW);
         digitalWrite(led_verde,LOW);
         digitalWrite(led_amarelo,HIGH);
       }else if(e==2){
         Negado();
         delay(2000);
                }
     else if(e==3){
         msgAceito();
       }}
void telaInicial(){
  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(13,15); //posicao do cursor no display           
  display.println("ROBOTICA"); //mensagem de texo a ser impressa na tela
  display.setCursor(15,45); //posicao do cursor no display           
  display.println("2CPM LDA"); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(2500);
  display.clearDisplay();
  display.display();
}   

void msgEspera(){
  display.clearDisplay();
  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_BLACK,WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(0,0); //posicao do cursor no display 
  display.println("  FECHADO ");
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(12,22);
  display.println(" Digite");
  display.setCursor(25,50);
  display.write(60);
  display.write(22);
  display.write(22);
  display.write(22);
  display.write(22);
  display.write(62);
  display.display();
  }
  void msgAceito(){
  display.clearDisplay();
  tone(pinoBuzzer,195,200);
  delay(250);
  tone(pinoBuzzer,195,200);

  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(28,20); //posicao do cursor no display           
  display.println("SENHA"); //mensagem de texo a ser impressa na tela
  display.setCursor(14,45); //posicao do cursor no display           
  display.println("CORRETA"); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  display.println("  ABERTA  "); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(1000);
  display.display();
}

void Negado(){
  display.clearDisplay();
  tone(pinoBuzzer,195,200);
  delay(250);
  tone(pinoBuzzer,195,200);

  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(28,20); //posicao do cursor no display           
  display.println("PORTA"); //mensagem de texo a ser impressa na tela
  display.setCursor(28,45); //posicao do cursor no display           
  display.println("FECHADA"); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  }
//display.write(22); escreve o caracter _
//display.write(26); escreve o caracter ->
//display.write(27); escreve o caracter <-
//display.write(35); escreve o caracter #
//display.write(42); escreve o caracter *
//display.write(60); escreve o caracter <
//display.write(61); escreve o caracter <
//display.write(62); escreve o caracter >
//display.write(129); escreve o caracter Ç
//display.write(167); escreve o caracter º
