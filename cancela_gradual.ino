#include <Servo.h>
#define pinoServo 7
Servo Cancela;

void setup(){
Cancela.attach(pinoServo);
}

void loop(){
  fechar();
  delay(3000);
  abrir();
  delay(3000);
}

void fechar(){
  for(int i=0;i<90;i++){
    Cancela.write(i);
    delay(15);
  }
}

void abrir(){
  for(int i=90; i>0; i--){
    Cancela.write(i);
    delay(15);
  }
}
