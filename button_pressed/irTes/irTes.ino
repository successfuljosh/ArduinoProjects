#define x 0

void setup() {
  // put your setup code here, to run once:
  pinMode(x, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(analogRead(x));
delay(100);

}

