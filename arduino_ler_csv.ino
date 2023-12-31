/*
** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

#define TIME 2000

String msg;
int temperatura = A0;
int luminosidade = A1;
int count = 1;
long tempo_atual;
long tempo_anterior = 0;

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  SD.remove("test.csv");
  myFile = SD.open("test.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.csv...");
    myFile.print("Medida; Timestamp; Temperatura; Luminosidade");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }


}

void loop() {

  tempo_atual = millis();

  if (tempo_atual - tempo_anterior >  TIME) {
    tempo_anterior = tempo_atual;
    int t = analogRead(temperatura);
    int l = analogRead(luminosidade);
    msg = String(count);
    msg += ";";
    msg += String(tempo_atual);
    msg += ";";
    msg += String(t);
    msg += ";";
    msg += String(l);
    myFile = SD.open("test.csv");
    myFile.println(msg);
    Serial.println(msg);
    myFile.close();
    count = count + 1;

  }
}

/*
  if (myFile) {
  //Serial.println("Medida; Timestamp; Temperatura; Luminosidade");
  // read from the file until there's nothing else in it:
  while (myFile.available()) {
    Serial.write(myFile.read());
  }
  // close the file:
  myFile.close();
  } else {
  // if the file didn't open, print an error:
  Serial.println("error opening test.txt");
  }


  //Serial.println(msg);
  }

  count++;

  //myFile.close();


  }*/


