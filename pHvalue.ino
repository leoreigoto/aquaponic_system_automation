
/*
#Este condigo de amostraé para testar o medidor de pH V1.0.
#Editor: YouYou
#Date: 213.10.21
#Ver : 0.1
#Product: pH meter
#SKU: SEN0161
 */

#define SensorPin 0 //Saída analógica do medidor pH para a entrada analogica ) do Arduino
#define Offset 0.00 // compessar desvio
unsigned long int avgValue; //Armazena o valor médio do feedback do sensor

void setup() {
pinMode(13,OUTPUT);
Serial.begin(9600);
Serial.println("ready"); // Teste de Serial Monitor
}

void loop() {
int buf[10]; // buffer para leitura analogica
for(int i=0;i<10;i++) //Obtenha 10 amostras do sensor para suavizar o valor
{
  buf[i]=analogRead(SensorPin);
  delay(10);
}
for(int i=0;i<9;i++) //classificar o analog de peq ao grd
{
  for(int j=i+1;j<10;j++)
  {
    if(buf[i]>buf[j])
    {
      int temp=buf[i];
      buf[i]=buf[j];
      buf[j]=temp;
      
    }
  }
}
avgValue=0;
for(int i=2;i<8;i++) //tome o valor medio de 6 amostras do centro
avgValue+=buf[i];
float phValue=(float)avgValue*5.0/1024/6; //convert o analogico para milivolt
phValue=3.5*phValue+Offset; // convert o millivolt para pH value
Serial.print("  pH:");
Serial.print(phValue,2);
Serial.println("  ");
digitalWrite(13,HIGH);
delay(800);
digitalWrite(13,LOW);

}
