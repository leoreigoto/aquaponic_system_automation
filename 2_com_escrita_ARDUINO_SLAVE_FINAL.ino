#include <Wire.h>

char t[10];
char c[10];

#define TEMPO 5000
//chave de nivel
#define pin1 3 // chave de nivel do reservatorio
#define pin2 4 // chave de nivel alto do aquario
#define pin3 5 // chave nivel baixo do aquario
#define out1 6 // solenoide do reservatório
#define out2 7 // solenoide do desvio do aquario
//temp
#include "Wire.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#define ONE_WIRE_BUS_1 4
OneWire ourWire1(ONE_WIRE_BUS_1);
DallasTemperature sensor1(&ourWire1);
//ph
#define AMOSTRAS 10
#define ARef  5.16    //TENSÃO DE REFERÊNCIA DO ARDUINO QUE USEI  --- MEDIR ENTRE O GND E AREF NO ARDUINO 
//servo motor
#include <Stepper.h>
#define TEMPO2 43200000// 12 horas em millis segundos
const int stepsPerRevolution = 500; 
//Inicializa a biblioteca utilizando as portas de 8 a 11 para 
//ligacao ao motor 
Stepper myStepper(stepsPerRevolution, 8,10,9,11);
//escrever csv
#include <SPI.h>
#include <SD.h>

int trocar=0;
long tempoanterior=0;
long tempoanterior2=0;
int ph_pin = A2; 
//escrever csv
String msg;
int count = 1;
File myFile;
 
void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);                
  Wire.onReceive(receiveEvent);     
  pinMode (pin1, INPUT);
pinMode (pin2, INPUT);
pinMode (pin3, INPUT);
pinMode (out1, OUTPUT);
pinMode (out2, OUTPUT);
 sensor1.begin();
  sensor1.setResolution(11);
   //Determina a velocidade inicial do motor 
 myStepper.setSpeed(60);
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

void loop() {
long tempoatual=millis();


if(tempoatual-tempoanterior>TEMPO)// ciclo de verificação a cada 5 seg para substituir o delay
{//if(tempoatual-tempoanterior>TEMPO)
  tempoanterior=tempoatual;
  
  
  int medida_ph = lePorta(ph_pin); 
  double voltage_ph =  (ARef / 1024.0) * medida_ph; 
  float Po = -6.45161290322581*voltage_ph + 23.4193548387097;  //EQUAÇÃO DA RETA
  Serial.print("\tPH: ");
  Serial.print(Po, 3);
  Serial.println("");


 msg = String(count);
    msg = String(count);
    msg += ";";
    msg += String(ValorCorrigido,1);
    msg += ";";
    msg += String(condutividade,3);
    msg += ";";
    msg += String(Po,3);
    myFile = SD.open("test.csv");
    myFile.println(msg);
    Serial.println(msg);
    myFile.close();
    count = count + 1;

  int a = digitalRead(pin1);// chave de nivel do reservatorio, se não me engano 1 é para ela aberta e 0 para ela fechada
int b = digitalRead(pin2);// nivel alto do aquario
int c = digitalRead(pin3);// nivel baixo do aquario
if(a=1)
{//if(a=1)
if(trocar<150)
{//if(trocar=<150)
  trocar=trocar+1;
  Serial.println("encher galão reservatório");
  Serial.println("Contagem de galões ");
  Serial.print(trocar);
}//if(trocar>150)
else{ trocar=0;}
  
}//if(a=1)

if(b=1)
{//if(b=1)
digitalWrite(out1,HIGH);//abre a solenoide do reservatorio para manter nivel alto do aquario
}//if(b=1)
else{digitalWrite(out1,LOW);}// quando o aquario estiver em nivel alto fecha a entrada de agua para o aquario vinda do reservatorio
if(c=1)
{//if(c=1)
digitalWrite(out2, LOW);// quando o aquario estiver em nivel baixo a solenoide de desvio é fechada e a de entrada de agua no aquario é aberta
digitalWrite(out1,HIGH);// abre a solenoide do reservatorio para o aquario, entra agua para aumentar o nivel do aquario
}//if(c=1);

}//if(tempoatual-tempoanterior>TEMPO)

if (tempoatual-tempoanterior2>TEMPO2)
{
tempoanterior2=tempoatual;
//Gira o motor no sentido horario a 90 graus
 for (int i = 0; i<=2; i++)
 {
 myStepper.step(-256); 
 delay(2000);
 }
  
 //Gira o motor no sentido anti-horario a 120 graus
 for (int i = 0; i<=2; i++)
 {
 myStepper.step(384); 
 delay(2000);
 }
}
if (c==0)
{
  if (ValorCorrigido<=20 || ValorCorrigido>=26)
{digitalWrite(out2,HIGH);
  digitalWrite(out1,HIGH);
}
if (Po<=6 || Po>=7.7)
{
  digitalWrite(out2,HIGH);
  digitalWrite(out1,HIGH);
}
}
if (ValorCorrigido>20 && ValorCorrigido<26)
{
  if (Po>6 && Po<7.7
  {
    digitalWrite(out2,LOW);
  }
}



}//void loop() 



 //função que recebe dados do esp8266
 
void receiveEvent(String leitura) {
     
    while (Wire.available()) {
        t[0] = Wire.read(); // every character that arrives it put in order in the empty array "t"
        t[1] = Wire.read();
        t[2] = Wire.read();
        t[3] = Wire.read();
        t[4] = Wire.read();         
        c[0] = Wire.read();
        c[1] = Wire.read();
        c[2] = Wire.read();
        c[3] = Wire.read();
        c[4] = Wire.read();
        c[5] = Wire.read();
       
    }
  
    Serial.println(t);  
    Serial.println(c);  
    delay(2000);
  
}