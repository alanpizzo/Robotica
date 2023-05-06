#include <Keypad.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)

Adafruit_SSD1306 display(128, 64, &Wire, -1);

#include <Keypad.h>
/* Inclui a bliblioteca de controle do Servo Motor.         */
#include<Servo.h>
/* Cria o objeto de controle do Servo Motor.                */
Servo servo;
/* Define a senha para destravar a fechadura.               */
char* password = "1951";
/* Quantidade de caracteres que a senha possui.             */
int caracteres = 4;
/* Define os pinos dos LEDs e Servo Motor.                  */
#define ledVermelho 12
#define ledVerde 11
#define Pin_Servo 10
/* Variável de controle.                                    */
int posicao = 0;
/* Define o número de linhas e colunas do teclado matricial */
/* de membrana.                                             */
#define qtdLinhas 4
#define qtdColunas 4
/* Construção da matriz de caracteres.                      */
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
/* Define os pinos de controle de linhas e colunas.         */
byte PinosqtdLinhas[qtdLinhas] = {9, 8, 7, 6};
byte PinosqtdColunas[qtdColunas] = {5, 4, 3, 2};
/* Cria o objeto de controle do teclado.                    */
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas);

void setup() {

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(500);
}

void loop() {
limpaTela();
}
void limpaTela(){
display.clearDisplay();
display.display();  
}
