// Ligando LED com push button
//  Autor: Luan Alves - www.guiarobotica.com
int pushbutton = 2; // declara o push button na porta 2
int led = 13; // declara led na porta 13
bool estadoled = 0; // variavel de controle

void setup()
{
  pinMode(pushbutton, INPUT_PULLUP); // define o pino do botao como entrada
  pinMode(led, OUTPUT);// define LED como saida
}

void loop()
{
  if (digitalRead(pushbutton) == LOW) // Se o botão for pressionado
  {
    estadoled = !estadoled; // troca o estado do LED
    digitalWrite(led, estadoled);
    while (digitalRead(pushbutton) == LOW);
    delay(100);
  }
}
