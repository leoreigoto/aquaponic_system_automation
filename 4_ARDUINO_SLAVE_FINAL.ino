#include <Wire.h>
#define TEMPO 2000
//chave de nivel
#define pin1 3 // chave de nivel do reservatorio
#define pin2 4 // chave de nivel alto do aquario
#define pin3 5 // chave nivel baixo do aquario
#define out1 6 // solenoide do reservatório
#define out2 7 // solenoide do desvio do aquario
char t[10];
char c[10];
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
int trocar=0;
long tempoanterior=0;
long tempoanterior2=0;
int ph_pin = A0; 
String temp;
String condut;
float condutividade=20;
float ValorCorrigido=25;
float Po=7;
int liberar=0;
void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);                
  Wire.onReceive(receiveEvent);     
   pinMode (pin1, INPUT);
pinMode (pin2, INPUT);
pinMode (pin3, INPUT);
pinMode (out1, OUTPUT);
pinMode (out2, OUTPUT);
   //Determina a velocidade inicial do motor 
 myStepper.setSpeed(60);
}

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

if (liberar==1)
{
if(tempoatual-tempoanterior>TEMPO)// ciclo de verificação a cada 5 seg para substituir o delay
{//if(tempoatual-tempoanterior>TEMPO)
  tempoanterior=tempoatual;
  
  
  int medida_ph = lePorta(ph_pin); 
  double voltage_ph =  (ARef / 1024.0) * medida_ph; 
  Po = -6.45161290322581*voltage_ph + 23.4193548387097;  //EQUAÇÃO DA RETA
  Serial.print("\tPH: ");
  Serial.print(Po, 3);
  Serial.println("");
  int a = digitalRead(pin1);// chave de nivel do reservatorio, se não me engano 1 é para ela aberta e 0 para ela fechada
int b = digitalRead(pin2);// nivel alto do aquario
int c = digitalRead(pin3);// nivel baixo do aquario
if(a==0)
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

if(b==0)
{//if(b=1)
digitalWrite(out1,HIGH);//abre a solenoide do reservatorio para manter nivel alto do aquario
}//if(b==1)
else{digitalWrite(out1,LOW);}// quando o aquario estiver em nivel alto fecha a entrada de agua para o aquario vinda do reservatorio
if(c==1)
{//if(c==0)
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
if (c==1)
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
  if (Po>6 && Po<7.7)
  {
    digitalWrite(out2,LOW);
  }
}

}
}



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
      temp=t[0]+t[1]+t[2]+t[3]+t[4]+t[5];
     condut=c[0]+c[1]+c[2]+c[3]+c[4]+c[5];
     ValorCorrigido=temp.toFloat();
   condutividade=condut.toFloat();
   liberar=1;
    }
  
    Serial.println(t);  
    Serial.println(c);  
    delay(2000);
  
}
