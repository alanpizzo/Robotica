#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Definição de pinos
#define RELAY_BOMBA 4
#define COOLER 5
#define DHTPIN 9
#define DHTTYPE DHT11
#define SOIL_SENSOR A0

// Inicialização do sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Variáveis de tempo
unsigned long tempoAnterior = 0;
const unsigned long intervalo = 2000; // Atualiza a cada 2 segundos

void setup() {
  pinMode(RELAY_BOMBA, OUTPUT);
  pinMode(COOLER, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

// Função para ler a umidade do solo
int lerUmidadeSolo() {
  int umidadeSolo = analogRead(SOIL_SENSOR);
  return map(umidadeSolo, 1023, 0, 0, 100); // Ajuste da escala do sensor
}

// Função para ler a temperatura e umidade do ar
void lerAmbiente(float &temperatura, float &umidadeAr) {
  temperatura = dht.readTemperature();
  umidadeAr = dht.readHumidity();
}

// Função para controlar a bomba de água
void controlarBomba(int umidade) {
  digitalWrite(RELAY_BOMBA, umidade < 30 ? HIGH : LOW);
}

// Função para controlar o cooler
void controlarCooler(float temperatura) {
  digitalWrite(COOLER, temperatura > 24 ? HIGH : LOW);
}

void loop() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual; // Atualiza o tempo de referência

    int umidadeSolo = lerUmidadeSolo();
    float temperatura, umidadeAr;
    lerAmbiente(temperatura, umidadeAr);

    Serial.print("Umidade do solo: ");
    Serial.print(umidadeSolo);
    Serial.print("% - Umidade do ar: ");
    Serial.print(umidadeAr);
    Serial.print("% - Temperatura: ");
    Serial.print(temperatura);
    Serial.println("°C");

    controlarBomba(umidadeSolo);
    controlarCooler(temperatura);
  }
}
