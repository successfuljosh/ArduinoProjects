#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//wifisection
//#define RX 3
//#define TX 2

String HOST = "api.thingspeak.com";
String PORT = "80";
String AP = "seeds";
String PASS = "1122334455";

String API = "QQ0DEXI5OVVC5LN1";
String field = "field1";

int countTrueCommand;
int countTimeCommand;
boolean found = false;

SoftwareSerial esp8266(3, 2);


//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial mySerial(4, 5);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define BACKLIGHT_PIN     13
int recordButton = 8;
int takeButton = 9;

uint8_t id = 1;
String matric[10] = {"130407019", "130407028", "130407038", "130407064", "130407033", "130407012", "130407060", "130407022", "130407029", "130407008"};

void RandGenerator() {
  for (int i = 0; i < 10; i++)
  {
    matric[i] = "1304070";
    matric[i] += random(99);
  }
}
void printAll(char text1[16], char text2[16] = " ")
{
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  Serial.println(text1);
  lcd.print(text1);
  Serial.println(text2);
  lcd.setCursor(0, 1);
  lcd.print(text2);
  delay(1500);
}


void setup()
{ randomSeed(10);
  //  RandGenerator();

  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  pinMode ( recordButton, INPUT );
  pinMode ( takeButton, INPUT );
  // digitalWrite(recordButton, HIGH);
  //  digitalWrite(takeButton, HIGH);
  lcd.begin(16, 2); //Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

}


void loop()                     // run over and over again
{    Serial.println();
//  for (int i = 0; i < 10; i += 2)
//  {
//    Serial.println(matric[i]);
//    Serial.println(matric[i + 1]);
//  }
  printAll("Select Mode");
  printAll("1. Record", "2. Attendance");
  //Recording Mode
  if (buttonPressed(recordButton) > 0) {
    printAll("Recording", "Mode::");
    printAll("Enroll a ", "fingerprint!");
    if (id < 10 ) {
      printAll("Enrolling ID for");
      lcd.clear();//Clean the screen
      lcd.setCursor(0, 0);
      Serial.println("Matric No: ");
      lcd.print("Matric No: ");
      lcd.setCursor(0, 1);
      Serial.println(matric[id]);
      lcd.print(matric[id]);
      delay(1000);
      while (!  getFingerprintEnroll() );
      id++;
    }

  }

  //attendance Mode
  else if (buttonPressed(takeButton) > 0) {
    printAll("Attendance", "Mode::");
    Serial.println(finger.getTemplateCount());
    printAll("Sensor contains ");
    lcd.clear();//Clean the screen
    lcd.setCursor(0, 0);
    Serial.println(finger.templateCount);
    lcd.print(finger.templateCount);
    lcd.setCursor(0, 1);
    Serial.println("templates");
    lcd.print("templates");
    delay(1000);
    printAll("Place finger", "for Attendance");
    delay(1000);
    while (! getFingerprintID());
    delay(50);
  }
}

int buttonPressed(int buttonPin)
{
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += digitalRead(buttonPin);
    Serial.println(digitalRead(buttonPin));
    delay(50);
  }
  int ave = sum / 7;
  Serial.print(ave);
  return ave;
}

//Matching region
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      printAll("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      printAll("No finger", " detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    printAll("Found a ", "print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    printAll("Communication", " error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    printAll("Did not find a ", "match");
    return p;
  } else {
    printAll("Unknown error");
    return p;
  }

  // found a match!
  printAll("Attendance"," Taken");
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  Serial.println("Matric No: ");
  lcd.print("Matric No: ");
  lcd.setCursor(0, 1);
  Serial.println(matric[finger.fingerID]);
  lcd.print(matric[finger.fingerID]);
  delay(1000);
    printAll("Uploading"," Data");
  Upload(matric[finger.fingerID]);
  return finger.fingerID;
}


uint8_t getFingerprintEnroll() {
  int p = -1;
//  printAll(id);
  printAll("place finger to", " enroll as #" + char(id));
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        { printAll("Image taken");
          digitalWrite ( BACKLIGHT_PIN, HIGH );
          delay(200);
          digitalWrite ( BACKLIGHT_PIN, LOW );
        }
        break;
      case FINGERPRINT_NOFINGER:
        printAll("place Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        printAll("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        printAll("Imaging error");
        break;
      default:
        printAll("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      printAll("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      printAll("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      printAll("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      printAll("Could not find", "fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      printAll("Could not find", "fingerprint features");
      return p;
    default:
      printAll("Unknown error");
      return p;
  }
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  delay(200);
  digitalWrite ( BACKLIGHT_PIN, LOW );
  printAll("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  lcd.setCursor(0, 1);
  Serial.println("ID " + matric[id]);
  lcd.print(matric[id]);
  delay(1000);
  p = -1;
  printAll("Place same", "finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        printAll("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        printAll("place Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        printAll("Communication", " error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        printAll("Imaging error");
        break;
      default:
        printAll("Unknown error");
        break;
    }
  }
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  delay(200);
  digitalWrite ( BACKLIGHT_PIN, LOW );
  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      printAll("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      printAll("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      printAll("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      printAll("Could not find", " fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      printAll("Could not find", "fingerprint features");
      return p;
    default:
      printAll("Unknown error");
      return p;
  }
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  delay(200);
  digitalWrite ( BACKLIGHT_PIN, LOW );
  // OK converted!
  printAll("Saving for", matric[id].c_str());
  printAll("Saving finger ", " print for: ");
  lcd.setCursor(0, 1);
  Serial.println(matric[id]);
  lcd.print(matric[id] + "  ");
  delay(1000);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    printAll("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    printAll("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    printAll("Fingerprints did", " not match");
    return p;
  } else {
    printAll("Unknown error");
    return p;
  }
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  delay(200);
  digitalWrite ( BACKLIGHT_PIN, LOW );
  lcd.setCursor(0, 1);
  Serial.println(matric[id]);
  lcd.print("ID: " + matric[id]);
  delay(1000);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    printAll("Stored!");
    Upload(matric[id]);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    printAll("Communication", " error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    printAll("Could not store", " in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    printAll("Error writing", " to flash");
    return p;
  } else {
    printAll("Unknown error");
    return p;
  }
}

void Upload(String matricNo)
{
  //initialize wifi
  esp8266.begin(115200);
  printAll("Starting Wifi", "Connection");
  sendCommand("AT", 5, "OK");
  sendCommand("AT+RST", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  Serial.println("Connecting to ");
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  Serial.println( AP + "...");
  lcd.print( AP + "...");
  delay(1000);
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 15, "OK");
  countTrueCommand = 0 ;

  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  Serial.println("Sending " + matricNo);
  lcd.print("Sending " + matricNo);
  lcd.setCursor(0, 1);
  Serial.println("to database");
  lcd.print("to database");
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
  printAll(command.c_str());
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

    printAll("DONE...");

    countTrueCommand++;
    countTimeCommand = 0;

  }
  if (found == false)
  {

    printAll("Connecting to ", "Server...");
    countTrueCommand = 0;
    countTimeCommand = 0;

  }
  found = false;

}
