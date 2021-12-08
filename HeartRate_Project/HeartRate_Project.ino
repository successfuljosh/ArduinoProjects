#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor;
int BPM, BloodPressure, RespiratoryRate;

SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+2348138596607";

int pressure = A5;

bool alert = false;
void setup() {
  delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("Sim Module Starting...");
  sim.begin(9600);
  pinMode(pressure, INPUT);

  Serial.println("GPS Module Starting...");
  ss.begin(GPSBaud);

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

  Serial.println();
  delay(1000);
  delay(1000);
}

void loop() {

  BPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
  // BloodPressure = 6270 / (BPM * 1.1585);
  BloodPressure = map(BPM, 50, 120, 100, 130);
  RespiratoryRate = map(analogRead(pressure), 0, 1023, 11, 30); //12-20 normal

  Serial.print("BPM: ");
  Serial.println(BPM);     // "myBPM" hold this BPM value now.
  Serial.print("RespiratoryRate: ");
  Serial.println(RespiratoryRate);     // "myBPM" hold this BPM value now.
  //Checking for Pulse and Blood Pressure
  if (pulseSensor.sawStartOfBeat())
  {
    if (BPM > 50 && BPM <= 110)
    { Serial.println("Pulse is normal");
      alert = false;
    }
    else if (BPM > 110)
    {
      Serial.println("Pulse is High");
      alert = true;
    }
    else if (BPM <= 50 && BPM > 30)
    {
      Serial.println("Pulse is LOW");
      alert = true;
    }
    else {
      Serial.println("Invalid BPM");
      alert = false;
    }

    //Checking for Respiratory Rate
    if (RespiratoryRate > 11 && RespiratoryRate <= 20)
    { Serial.println("Respiratory Rate is normal");
      alert = false;
    }
    else if (RespiratoryRate > 20)
    {
      Serial.println("RespiratoryRate is High");
      alert = true;
    }
    else if (RespiratoryRate <= 11)
    {
      Serial.println("RespiratoryRate is LOW");
      alert = true;
    }
    else
    {
      Serial.println("Invalid Respiratory Rate");
      alert = false;
    }
  //  alert = true;
  }
  if (alert == true) {
    SendMessage();
    alert = false;
  }
  //
  //  if (pulseSensor.sawStartOfBeat())
  //  { // Constantly test to see if "a beat happened".
  //    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
  //    Serial.print("BPM: ");                        // Print phrase "BPM: "
  //    Serial.println(BPM);                        // Print the value inside of myBPM.
  //  }
  delay(2000);
}


String Location()
{ String loca = "http://maps.google.com/?ll=6.5176275,3.4030608";
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 3);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 3);
      loca = "http://maps.google.com/?ll=";
      loca += gps.location.lat();
      loca += ",";
      loca += gps.location.lng();
    }
    else {
      loca = "http://maps.google.com/?ll=6.5176275,3.4030608";
    }
  }

  return loca;
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
  String SMS = "Doctor's Alert!! \nBPM: ";
  SMS += BPM;
  SMS += ", BP: ";
  SMS += BloodPressure;
  SMS += ", Respiratory Rate: ";
  SMS += RespiratoryRate;
  SMS += "\nClick here to get Location of patient\n";
  SMS += Location();

  sim.println(SMS);
  Serial.println(SMS);
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

