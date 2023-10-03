/*

CALCULA TEMPERATURA DA SOLUÇÃO

*/

#include "Wire.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#define ONE_WIRE_BUS_1 4
OneWire ourWire1(ONE_WIRE_BUS_1);
DallasTemperature sensor1(&ourWire1);

float Valor = 0.0;
float MedidoAlto = 99.8;
float MedidoBaixo = 0.1;
float RefAlto = 100.0;
float RefBaixo = 0.0;
float Faixa = MedidoAlto - MedidoBaixo;
float RefFaixa = RefAlto - RefBaixo;


void setup(){

  Serial.begin(9600);
  sensor1.begin();
  sensor1.setResolution(11);

}
void loop(){
  sensor1.requestTemperatures();
  float ValorLido = sensor1.getTempCByIndex(0);
  float ValorCorrigido = (((ValorLido - MedidoBaixo) * RefFaixa) / Faixa) + RefBaixo;
  Serial.print("Temperatura(f): ");
  Serial.print(ValorCorrigido, 1);
  Serial.println(" °C");
  delay(2000);
}
