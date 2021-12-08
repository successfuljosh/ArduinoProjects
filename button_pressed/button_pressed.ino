void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  Serial.begin(9600);
//  digitalWrite(8, LOW);
  //digitalWrite(9, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonPressed(8)) {
    Serial.println("On pressed");
  }
  else if (buttonPressed(9)) {
    Serial.println("off pressed");
  } else {
    Serial.println("rhg");
  }
   delay(100);
}

int buttonPressed(int buttonPin)
{
  int sum = 0;
  for (int i = 0; i < 150; i++) {
    sum += digitalRead(buttonPin);
    //delay(50);
  }
  int ave = sum / 50;
  if (ave >= 1) return true;
  return false;
}

