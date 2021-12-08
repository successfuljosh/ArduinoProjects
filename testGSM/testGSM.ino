#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 11
#define FONA_TX 10
#define FONA_RST 5

char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void setup() {
  while (!Serial);
  Serial.begin(115200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));
  fonaSS.begin(4800); 
  if (! fona.begin(fonaSS)) {            
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));

   char sendto[21] = "+2348138596607", message[141] = "Hello there, Testing GSM Module";
   flushSerial();
   Serial.print(F("Send to #"));
   Serial.println(sendto);
   Serial.print(F("Sending message: "));
   Serial.println(message);
   if (!fona.sendSMS(sendto, message)) {
      Serial.println(F("Failed"));
   } else {
      Serial.println(F("Sent!"));
   }
}

void loop() {}

void flushSerial() {
    while (Serial.available()) 
    Serial.read();
}

