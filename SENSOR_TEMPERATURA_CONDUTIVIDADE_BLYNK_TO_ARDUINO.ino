/* ESP & Blynk */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <Wire.h>
char auth[] = "d0dc5dda8af741b7b00e3ab4aa0f63f8";

/* WiFi credentials */
char ssid[] = "DayseEThiago";
char pass[] = "winterfell";

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;

/* DS18B20 Temperature Sensor */
#include <OneWire.h>
#include<DallasTemperature.h> 
#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor1(&oneWire);


int temp_0;
float Valor = 0.0;
float ValorCorrigido = 0.0;
float ValorLido = 0.0;
float MedidoAlto = 99.8;
float MedidoBaixo = 0.1;
float RefAlto = 100.0;
float RefBaixo = 0.0;
float Faixa = MedidoAlto - MedidoBaixo;
float RefFaixa = RefAlto - RefBaixo;


/* Condutividade */
int cond_pin = A0; 
#define AMOSTRAS 10
const float ARef = 3.3;
byte medida_cond; 
float voltage_cond; 
float condutividade;
float cond_blynk;


void setup() 
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  sensor1.begin();
  sensor1.setResolution(11);
  Wire.begin();
  timer.setInterval(2000L, getSendData);
  Serial.println(" ");

}

void loop() 
{
  timer.run(); 
  Blynk.run();
}

  //FUNÇÃO QUE CALCULA A MÉDIA DE 10 LEITURAS DA PORTA ANALÓGICA

float lePorta(int portaAnalogica) {
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
    delay(5);
  }
  return total / (float)AMOSTRAS;
}  

  //FUNÇÃO QUE ENVIA DADOS PARA O BLYNK E PARA O ARDUINO

void getSendData()
{

  char s1[10];
  char s2[10]; 
    
    

  //temperatura
  sensor1.requestTemperatures();
  ValorLido = sensor1.getTempCByIndex(0);
  ValorCorrigido = (((ValorLido - MedidoBaixo) * RefFaixa) / Faixa) + RefBaixo;

  //condutividade
  medida_cond = lePorta(cond_pin); 
  voltage_cond =  (ARef / 1024.0) * medida_cond; 
  condutividade = 20043.1340642878*voltage_cond - 658.582258210324;  //EQUAÇÃO DA RETA
  cond_blynk = condutividade/1000;
   

  //converte temp. e condut. para string 
  dtostrf(ValorCorrigido,5, 2, s1);
  dtostrf(condutividade,6, 1, s2);

   
  Wire.beginTransmission(0x08);    //endereço do Arduino pelo protocolo Wire 
  Wire.write(strcat(s1,s2));      //concatena a temperatura e a condutividade e envia como uma string única          
  Wire.endTransmission();
  

  Blynk.virtualWrite(10, ValorCorrigido); //virtual pin V10
  Blynk.virtualWrite(11,cond_blynk  ); //virtual pin V11
}
