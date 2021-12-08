
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define s    13

void printAll(LiquidCrystal_I2C lcd, String text1)
{
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  Serial.println(text1);
  lcd.print(text1);
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
  pinMode(s, OUTPUT);
}


void loop()                     // run over and over again
{
  printAll(lcd2, "Hello one");
  delay(1000);
  digitalWrite(s, HIGH);
  delay(500);
  digitalWrite(s, LOW);
  delay(500);
  printAll(lcd1, "Hello two");
  delay(1000);
  digitalWrite(s, HIGH);
  delay(500);
  digitalWrite(s, LOW);
  delay(500);
}
