/*==================================================================================
=                                                                                  =
=                      B R A Ç O     R O B Ó T I C O                               =
=                                                                                  =
====================================================================================*/

/*Bibliotecas
===========================================*/
#include <Servo.h> //inclusão da biblioteca para os servos motores


/*Variáveis e definições
===========================================*/
#define eixox1 14 //define o pino do eixo x1 do joystick
#define eixoy1 15 //define o pino do eixo y1 do joystick
#define eixox2 16 //define o pino do eixo x2 do joystick
#define eixoy2 17 //define o pino do eixo y2 do joystick

int anguloBase=90;  //inicia o ângulo da base em 90°
int anguloGarra=0;  //inicia o ângulo da garra em 0°
int anguloDireito=90;  //inicia o ângulo do servo direito em 90°
int anguloEsquerdo=90;  //inicia o ângulo ddo servo esquerdo em 90°

int potGarra;
int potBase;
int potEsquerdo;
int potDireito;

/*Instanciar motores
===========================================*/
Servo Garra;
Servo Base;
Servo Direito;
Servo Esquerdo;

void setup() {
Serial.begin(9600);
/* Definição dos pinos de cada servo motor
==========================================*/
Garra.attach(3);
Base.attach(5);
Direito.attach(7);
Esquerdo.attach(9);

/*Ângulos iniciais de cada servo motor
=========================================*/
Garra.write(anguloGarra);
Base.write(anguloBase);
Esquerdo.write(anguloEsquerdo);
Direito.write(anguloDireito);
}
void loop() {
  int potGarra = analogRead(eixox1);
  int potBase = analogRead(eixoy1);
  int potDireito = analogRead(eixox2);
  int potEsquerdo = analogRead(eixoy2);

if(potGarra>400){
    anguloGarra=anguloGarra+10;
    delay(100);
  }
  if(potGarra<200){
    anguloGarra=anguloGarra-10;
    delay(100);
  }
Garra.write(anguloGarra);

if(potBase>400){
    anguloBase=anguloBase+10;
    delay(100);
  }
  if(potBase<200){
    anguloBase=anguloBase-10;
    delay(100);
  }
Garra.write(anguloBase);

Serial.print("Angulo da garra: ");
Serial.print(anguloGarra);
Serial.print(" / Potenciometro da garra: ");
Serial.println(potGarra);

Serial.print("Angulo da base: ");
Serial.print(anguloBase);
Serial.print(" / Potenciometro da base: ");
Serial.println(potBase);
delay(100);
}

void movimentoGarra(){
  
}
