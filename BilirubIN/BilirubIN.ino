
#include <LiquidCrystal_I2C.h>
#define buzzer 5//6
#define yellow 13
#define red 11
#define green 12
#define fullGreen 7
#define batteryLevel A1
#define photosensor A3
#define NUM_SAMPLES 10

int sum = 0;
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;
int percentage = 0;
//I2C pins declaration
LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define BACKLIGHT_PIN     13

void printAll(LiquidCrystal_I2C lcd, String text1, char text2[16] = " ")
{
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  Serial.println(text1);
  lcd.print(text1);
  Serial.println(text2);
  lcd.setCursor(0, 1);
  lcd.print(text2);
  delay(1000);
}


void setup()
{
  Serial.begin(9600);

  // set the data rate for the sensor serial port
  lcd1.begin(16, 2); //Defining 16 columns and 2 rows of lcd display
  lcd1.backlight();//To Power ON the back light
  lcd2.begin(16, 2); //Defining 16 columns and 2 rows of lcd display
  lcd2.backlight();//To Power ON the back light
  pinMode(buzzer, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(fullGreen, OUTPUT);
  pinMode(photosensor, INPUT);
  printAll(lcd2, "Bilirubin", "Detector");
  printAll(lcd1, "Battery", "Information");


  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
  delay(1000);
  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);
  delay(1000);

}


void loop()                     // run over and over again
{
  // Serial.println(analogRead(photosensor));
  int light = map(analogRead(photosensor), 0, 800, 0, 500);
  if (light > 60)
  {
    String nm = String(map(light, 80, 500, 450, 490)) + "nm";
    lcd2.clear();//Clean the screen
    lcd2.setCursor(0, 0);
    Serial.println(light);
    lcd2.print("Wavelength:");
    Serial.println(analogRead(photosensor));
    lcd2.setCursor(0, 1);
    lcd2.print(nm);
    delay(1000);
  }
  else if (light < 80 && light > 20) {
    //    lcd.clear();//Clean the screen
    //    lcd.setCursor(0, 0);
    //    Serial.println("Place under Blue LED");
    //    lcd.print("Level:");
    //    lcd.setCursor(0, 1);
    //    lcd.print(light);
    //    delay(500);
    String bili = String(map(light, 20, 60, 2, 21)) + "mg/dl";
    if (light > 20 && light < 25) {
      printAll(lcd2, "Normal");
      digitalWrite(green, HIGH);
      lcd2.setCursor(0, 1);
      lcd2.print(bili);
      delay(1000);
    } else if (light > 25 && light < 45) {
      printAll(lcd2, "Mild");
      digitalWrite(yellow, HIGH);
      lcd2.setCursor(0, 1);
      lcd2.print(bili);
      delay(1000);
    } else if (light > 45 && light < 60) {
      printAll(lcd2, "Critical");
      lcd2.setCursor(0, 1);
      lcd2.print(bili);
      digitalWrite(red, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1000);
    }

  }
  else {
    lcd2.clear();//Clean the screen
    lcd2.setCursor(0, 0);
    Serial.println("Place under Blue LED");
    lcd2.print("Place under");
    lcd2.setCursor(0, 1);
    lcd2.print("Photo-Therapy");
  }


  //  if (light > 66) {
  //    printAll("Bright Light", "Detected");
  //    digitalWrite(green, HIGH);
  //    delay(1000);
  //  } else if (light > 33) {
  //    printAll("Normal Light", "Detected");
  //
  //    digitalWrite(yellow, HIGH);
  //    delay(1000);
  //  }
  //  else {
  //    printAll("LOW Light", "Detected");
  //    digitalWrite(red, HIGH);
  //    digitalWrite(buzzer, HIGH);
  //    delay(1000);
  //  }

  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(buzzer, LOW);
  delay(10);

  //  delay(1000);
  //  if (light > 390)
  //  {
  //    printAll("WaveLength");
  //  }

  delay(1000);
  BatteryStatusAndLevel();
}

void BatteryStatusAndLevel()
{
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(batteryLevel);
    sample_count++;
    delay(10);
  }
  // calculate the voltage
  // use 5.0 for a 5.0V ADC reference voltage
  // 5.015V is the calibrated reference voltage
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1024.0;
  // send voltage for display on Serial Monitor
  // voltage multiplied by 11 when using voltage divider that
  // divides by 3.2727 which is the calibrated voltage divide value
  voltage = voltage * 3.2727;
  percentage = map(voltage, 6, 12, 10, 100); //50% of total battery capacity is useful
  sample_count = 0;
  sum = 0;

  if (percentage > 99) {
    //Battery fully charged
    lcd1.clear();//Clean the screen
    lcd1.setCursor(0, 0);
    Serial.print("Battery Fully charged:");
    Serial.print(percentage);
    Serial.print("%");
    lcd1.print("Fully charged!");
    lcd1.setCursor(0, 1);
    lcd1.print(percentage);
    lcd1.setCursor(5, 1);
    lcd1.print("%");
    delay(100);
    digitalWrite(fullGreen, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(fullGreen, LOW);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(fullGreen, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
  }
  else if (percentage < 20) {
    //Battery low
    lcd1.clear();//Clean the screen
    lcd1.setCursor(0, 0);
    Serial.print("Battery Very Low:");
    Serial.print(percentage);
    Serial.print("%");
    lcd1.print("Low Battery!");
    lcd1.setCursor(0, 1);
    lcd1.print(percentage);
    lcd1.setCursor(5, 1);
    lcd1.print("%");
    delay(100);
    //  digitalWrite(red, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(1000);
  }
  else {
    //Print
    lcd1.clear();//Clean the screen
    lcd1.setCursor(0, 0);
    Serial.print("Battery Level:");
    Serial.print(percentage);
    Serial.print("%");
    lcd1.print("Battery Level:");
    lcd1.setCursor(0, 1);
    lcd1.print(percentage);
    lcd1.setCursor(5, 1);
    lcd1.print("%");
    delay(1000);
    //Print
    lcd1.clear();//Clean the screen
    lcd1.setCursor(0, 0);
    Serial.print("Battery Voltage:");
    Serial.print(voltage);
    Serial.print("V");
    lcd1.print("Battery Voltage:");
    lcd1.setCursor(0, 1);
    lcd1.print(voltage);
    lcd1.setCursor(5, 1);
    lcd1.print("V");
    delay(1000);
  }
  digitalWrite(fullGreen, LOW);
  digitalWrite(buzzer, LOW);
  delay(1000);
}


