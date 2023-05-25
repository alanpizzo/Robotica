/*=====================================================================================================================================
=                                      R O B O T I C A    2 C P M      L O N D R I N A                                                =
=                                                                                                                                     =
=                                              C E L U L A R     F A N T A S M A                                                      =
=======================================================================================================================================*/

#define pinoMotor 8 //pino de dados do motor de vibração
#define pinoSensor 9 //pino do módulo sensor sensor LDR
bool leitSensor; //estado do sensor LDR HIGH(escuro) e LOW(claro).
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//inicia comunicação serial
  pinMode(pinoSensor,INPUT); //modo do pino do sensor
  pinMode(pinoMotor, OUTPUT); // modo do pino do motor
}

void loop() {
  // put your main code here, to run repeatedly:
  leitSensor = digitalRead(pinoSensor); //lê sensor e armazena na variável
  if (leitSensor) { //se escuro
    digitalWrite(pinoMotor, HIGH); //motor vibra
  }
  else {
    digitalWrite(pinoMotor, LOW);//se claro
  }
  Serial.println(leitSensor); //motor para de vibrar
}
