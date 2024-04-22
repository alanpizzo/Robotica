#include <Adafruit_NeoPixel.h>

#define PIN        6  // pino de controle da fita de led
#define NUMPIXELS 300 // número de leds presentes na fita

/*Instancia a fita de led
======================================================================= */
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//#define DELAYVAL 0 // Time (in milliseconds) to pause between pixels

void setup() {
  pixels.begin(); // Inicializa a fita de led
  pixels.clear(); // apaga todos os leds

}

void loop() {
  verde();
  delay(10000);
  amarelo();
  delay(4000);
  vermelho();
  delay(10000);
}
/*função para acendimento do sinal vermelho dos carros*/
void vermelho(){
   pixels.clear(); // apaga todos os leds
  /*acende metade dos leds vermelhos*/
  for(int i=0; i<15; i++) { 
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();   // executa o acendimento dos leds na cor desejada.
    }//fim do for
  /*acende a outra metade dos leds vermelhos*/  
    for(int i=19; i<35; i++) { 
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();   // executa o acendimento dos leds na cor desejada.
    } 
}//fim função vermelho

/*função para acendimento do sinal vermelho dos carros*/
void amarelo(){
    pixels.clear(); // apaga todos os leds
    for(int i=44; i<60; i++) { 
    pixels.setPixelColor(i, pixels.Color(150, 150, 0));
    pixels.show();   // executa o acendimento dos leds na cor desejada.
    }//fim do laço for
}//fim função amarelo

void verde(){
     pixels.clear(); // apaga todos os leds
    for(int i=64; i<80; i++) { 
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();   // executa o acendimento dos leds na cor desejada.
    }//fim do laço for
}//fim função verde
