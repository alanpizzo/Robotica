/*Bibliotecas para utilizar o visor OLED
===================================================================================================================*/
#include <SPI.h>
#include <Wire.h> // define as portas do display A4(SDA), A5(SCL)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*Cria o objeto display OLED 128X64 nas portas A4 e A5 no início da tela à esquerda (-1)
===================================================================================================================*/
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicia o diplay OLED
limpaTela(); //chama a função para limpar as informações do display

}

void loop() {
  display.setTextSize(2);      // tamanho do texto no display      
  display.setTextColor(SSD1306_WHITE);        //cor do texto, pode ser alternado para SSD1306_BLACK,WHITE
  display.setCursor(21,0); //posicao do cursor no display           
  display.println("ROBOTICA CPM!"); //mensagem de texo a ser impressa na tela
  display.display(); //manda as informações para o visor
  delay(1000);
}
void limpaTela(){ //funcao para facilitar a limpeza de imformações do display
display.clearDisplay();
display.display();  
}
