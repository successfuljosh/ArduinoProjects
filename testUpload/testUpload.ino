#include <SoftwareSerial.h>
//wifisection
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

SoftwareSerial esp8266(RX, TX);

void setup() {
  // put your setup code here, to run once:
Upload("130407063");
}

void Upload(String matricNo)
{
  //initialize wifi
  esp8266.begin(115200);
 // printAll("Starting Wifi", "Connection");
  sendCommand("AT", 5, "OK");
  sendCommand("AT+RST", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
 // lcd.clear();//Clean the screen
//  lcd.setCursor(0, 0);
  Serial.println("Connecting to ");
//  lcd.print("Connecting to");
//  lcd.setCursor(0, 1);
  Serial.println( AP + "...");
///  lcd.print( AP + "...");
  delay(1000);
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 15, "OK");
  countTrueCommand = 0 ;



 // lcd.clear();//Clean the screen
 // lcd.setCursor(0, 0);
  Serial.println("Sending " + matricNo);
//  lcd.print("Sending " + matricNo);
//  lcd.setCursor(0, 1);
  Serial.println("to database");
//  lcd.print("to database");
  delay(1000);
  String getData = "GET https://" + HOST + "/update?api_key=" + API + "&" + field + "=" + matricNo;
  sendCommand("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=" + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); delay(1500); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  Serial.print(getData);
  Serial.print(",");
  Serial.println(getData.length());
  delay(5000);
}
void sendCommand(String command, int maxTime, char readReplay[]) {

  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
 // printAll(command.c_str());
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

  //  printAll("DONE...");

    countTrueCommand++;
    countTimeCommand = 0;

  }
  if (found == false)
  {

   // printAll("An Error occured", "Retrying..");
    countTrueCommand = 0;
    countTimeCommand = 0;

  }
  found = false;

}

void loop() {
  // put your main code here, to run repeatedly:

}
