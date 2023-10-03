/*

CALCULA A CONDUTIVIDADE DA SOLUÇÃO

*/
int cond_pin = A1; 
#define AMOSTRAS 10
#define ARef  5.16        //TENSÃO DE REFERÊNCIA DO ARDUINO QUE USEI  --- MEDIR ENTRE O GND E AREF NO ARDUINO 


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

  int medida_cond = lePorta(cond_pin); 
  double voltage_cond =  (ARef / 1024.0) * medida_cond; 
  float condutividade = 42994,0268790443*voltage_cond -1245,89347934296;  //EQUAÇÃO DA RETA
  Serial.print("\tCONDUTIVIDADE: ");
  Serial.print(condutividade, 3);
  Serial.println(" uS/cm");
  delay(2000);

}
