/*===================================================================================

                  E S T A Ç Ã O     M E T E O R O L Ó G I C A

=====================================================================================
Inclusão de bibliotecas
===============================*/
#include<WiFi.h>              // biblioteca para rede wifi
#include<PubSubClient.h>      // biblioteca para comunicação MQTT
#include <DHT.h>              // biblioteca do sensor DHT11
#include <DHT_U.h>            // biblioteca do sensor DHT11

/*Instanciação de objetos
===============================*/
DHT dht(4, DHT11);            // instancia o sensor de umidade e temperatura

/*Credenciais da rede Wifi
===============================*/
const char* ssid = "CLARO_EF05E3";
const char* password = "ZqYMJvXWT7";

/*Configurações do Broker MQTT
===============================*/
const char* mqtt_server = "broker.hivemq.com";
WiFiClient espCofre;
PubSubClient client(espCofre);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

/*Variáveis e definições
===============================*/
char LED;
char Sensor;


/*função conexão da rede Wifi
===============================*/
void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Conectando rede:");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

Serial.println("");
Serial.println("WiFi conectado");
Serial.println("Endereço IP");
Serial.println(WiFi.localIP());
} //fim da função setup_wifi

/* Função para reconexão ao broker MQTT
===============================*/
void reconnect(){
while(!client.connected()){
  Serial.print("Tentando conexão com o broker MQTT");
  String clientID = "CPMcofre";
  clientID += String(random(0xffff),HEX);

  if(client.connect(clientID.c_str())){
    Serial.println("Conectado");

    client.subscribe("cpm/estacao");

  }//fim do laço if
  else{
    Serial.print("falha, rc=");
    Serial.print(client.state());
    Serial.println("tentar de novo em 5 segundos");
    delay(5000);
  }//fim do laço else

}//fim do laço while
}//fim da função reconnect

void setup() {

  Sensor = 4; //definição da porta do sensor

  pinMode(Sensor, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
  dht.begin();
}
void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String umidade=String(h);
  String temperatura=String(t);
  Serial.print("Umidade: ");
  Serial.println(umidade);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  sprintf(msg,"%f",t);
  client.publish("cpm/estacao/temperatura", msg);
  sprintf(msg,"%f",h);
  client.publish("cpm/estacao/umidade", msg);
  delay(1000);

  if(!client.connected()){
    reconnect();
  }//fim do laço if
  client.loop();
}//fim do loop
