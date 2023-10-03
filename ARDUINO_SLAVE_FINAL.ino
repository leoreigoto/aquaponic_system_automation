#include <Wire.h>

char t[10];
char c[10];

 
void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);                
  Wire.onReceive(receiveEvent);     
       
}
 
void loop() {
  delay(100);
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
       
    }
  
    Serial.println(t);  
    Serial.println(c);  
    delay(2000);
  
}
