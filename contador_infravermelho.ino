/*=======================================================================================================================
  =                                    R O B O T I C A     2 C P M    L O N D R I N A                                     =
  =========================================================================================================================
*/
#define pinoIV 8
bool SensorIV = 0;
unsigned long contador = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pinoIV, INPUT);

}// fecha o Setup

void loop() {
  Serial.println(contador);
  
if (!digitalRead(pinoIV)) {
    SensorIV = 1;
  }
  if (digitalRead(pinoIV) && SensorIV) {
    SensorIV = 0;
    contador += 1;
  }
  
}// fecha o Loop
