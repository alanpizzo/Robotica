/*=======================================================================================================================================
=                                     R O B Ó T I C A     2  C P M - L O N D R I N A                                                    =
=                                                                                                                                       =
=                                           P U S H B U T T O N                                                                         =
=========================================================================================================================================*/

/* Variáveis e definições
=========================================================================================================================================*/
#define pinoBT  2 // define o pino do botão
#define pinoLed  13 // define o pino do led
bool estadoLed = LOW; // declara a variável que vai controlar o estado do led

void setup(){
  pinMode(pinoBT, INPUT_PULLUP); // define o modo do pino do botão 
  pinMode(pinoLed, OUTPUT);// define o pino do led como saída
}

void loop(){
  if (digitalRead(pinoBT) == LOW) // Se o botão for pressionado
  {
    estadoLed = !estadoLed; // troca o estado do LED
    digitalWrite(pinoLed, estadoLed);
    while (digitalRead(pinoBT) == LOW);
    delay(50);
  }
}
