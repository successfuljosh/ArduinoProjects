int pressure = A5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pressure, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(map(analogRead(pressure), 0, 1023, 12, 30));
  delay(1000);
}
