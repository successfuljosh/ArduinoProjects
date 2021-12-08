#include <SoftwareSerial.h>
#define DEBUG true
#define RX 3
#define TX 2

String HOST = "api.thingspeak.com";
String PORT = "80";
String AP = "seeds";
String PASS = "1122334455";


String API = "QQ0DEXI5OVVC5LN1";
String field = "field1";

int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;

SoftwareSerial esp8266(RX, TX);
void setup() {

  Serial.begin(9600);
  esp8266.begin(115200);

  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=3", 5, "OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"", 15, "OK");
  countTrueCommand = 0;

}

void loop() {

  String getData = "GET https://"+ HOST +"/update?api_key=" + API + "&" + field + "=heyyo Newe";
  switch (countTrueCommand) {

    case 0: sendCommand("AT", 5, "OK"); break;
   // case 1: sendCommand("AT+RST", 10, "invalid"); break;
    case 2: sendCommand("AT+CIPMUX=1", 5, "OK"); break;
    case 3: sendCommand("AT+CIPSTART=,\"TCP\"," + HOST + "\",\"" + PORT, 15, "OK"); break;
    case 4: sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">"); break;
    case 5: esp8266.println(getData); delay(1500); countTrueCommand++; break;
    case 6: sendCommand("AT+CIPCLOSE=0", 5, "OK"); break;


      Serial.print(getData);
      Serial.print(",");
      Serial.println(getData.length());
     countTrueCommand = 0;
      delay(10000);
      break;

  }

}
void sendCommand(String command, int maxTime, char readReplay[]) {

  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))

  {

    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {

      found = true;
      break;

    }
    countTimeCommand++;

  }
  if (found == true)
  {

    Serial.println("OYI");

    countTrueCommand++;
    countTimeCommand = 0;

  }
  if (found == false)
  {

    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;

  }
  found = false;

}
