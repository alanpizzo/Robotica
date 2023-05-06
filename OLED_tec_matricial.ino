#include <Keypad.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)

Adafruit_SSD1306 display(128, 64, &Wire, -1);

/* Keypad setup */
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {9, 8, 7, 6};
byte colPins[KEYPAD_COLS] = {5, 4, 3, 2};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() {
Serial.begin(9600);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
display.clearDisplay();
display.display();
}

void loop() {
showStartupMessage();
limpaTela();
inputSecretCode();
limpaTela();
Serial.println(keypad.getKey());
}
void limpaTela(){
display.clearDisplay();
display.display();  
}
void showStartupMessage(){
  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(21,0);           
  display.println("Welcome!");
  display.display();
  delay(1000);

  display.setTextSize(2);
  display.setCursor(5, 42);
  String message = "Cofre 2CPM";
  for (byte i = 0; i < message.length(); i++) {
    display.print(message[i]);
    display.display();
    delay(100);
  }
  delay(500);
}
String inputSecretCode() {
  display.setTextSize(2);
  display.setCursor(15, 10);
  display.print("[____]");
  display.display();

  
  display.setTextSize(2);
  display.setCursor(17, 10);
  String result = "";
  while (result.length() < 4) {
    char key = keypad.getKey();
    if (key == '0' && key == '1'&& key == '2'&& key == '3'&& key == '4'&& key == '5'&& key == '6'&& key == '7'&& key == '8'&& key == '9') {
      display.print(key);
      display.display();
      result += key;
    }
  }
  return result;
  display.display();

  Serial.print(result);
}
