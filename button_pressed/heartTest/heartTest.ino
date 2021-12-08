int a0 = 0;



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(a0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(analogRead(a0));
delay(100);
}
