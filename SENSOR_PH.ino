/*
CALCULA O PH DA SOLUÇÃO 
 */

int ph_pin = A0; 
#define AMOSTRAS 10
#define ARef  5.16    //TENSÃO DE REFERÊNCIA DO ARDUINO QUE USEI  --- MEDIR ENTRE O GND E AREF NO ARDUINO 

void setup() {
  Serial.begin(9600);
 
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

  int medida_ph = lePorta(ph_pin); 
  double voltage_ph =  (ARef / 1024.0) * medida_ph; 
  float Po = -6,45161290322581*voltage_ph + 23,4193548387097;  //EQUAÇÃO DA RETA
  Serial.print("\tPH: ");
  Serial.print(Po, 3);
  Serial.println("");
  delay(2000);
}
