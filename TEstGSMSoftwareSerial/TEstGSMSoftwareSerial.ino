#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+2348138596607";

void setup() {
  delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("System Started...");
  sim.begin(9600);
  delay(5000);
}

void loop() {
  //Serial.println(_buffer);
  SendMessage();
  Serial.println(_buffer);// 
  delay(5000);
}

void SendMessage()
{
 Serial.println ("Sending Message");
  sim.println("AT");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  Serial.println("AT+CMGS=\"" + number + "\"\r");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "Hello there, Testing GSM Module";
  sim.println(SMS);
  Serial.println("Message Sent");
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

void callNumber() {

  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
    Serial.println ("Calling");
    _buffer = _readSerial();
  Serial.println(_buffer);
}


void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  delay (1000);
  sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  Serial.write ("Unread Message done");
}
