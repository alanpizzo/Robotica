
#include <Ultrasonic.h>
#define pinEcho 12
#define pinTrig 11
#define pinoVibra 7

/*
 * Pass as a parameter the trigger and echo pin, respectively,
 * or only the signal pin (for sensors 3 pins), like:
 * Ultrasonic ultrasonic(13);
 */
Ultrasonic ultrasonic(pinTrig, pinEcho);
int distance;

void setup() {
  pinMode(pinoVibra,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(100);
  if(distance<=30){
    digitalWrite(pinoVibra,HIGH);
    delay(1000);
    digitalWrite(pinoVibra,LOW);
  }else if(distance>30&&distance<70){
    digitalWrite(pinoVibra,HIGH);
    delay(200);
    digitalWrite(pinoVibra,LOW);
    delay(200);
    digitalWrite(pinoVibra,HIGH);
    delay(200);
    digitalWrite(pinoVibra,LOW);
  } else if(distance>=100&&distance<150){
    digitalWrite(pinoVibra,HIGH);
    delay(200);
    digitalWrite(pinoVibra,LOW);
    delay(200);
  }

  
}
