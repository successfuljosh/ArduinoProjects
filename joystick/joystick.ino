#define x 0
#define left  7
#define right 8
#define spee 255

void setup() {
  // put your setup code here, to run once:
  pinMode(x, INPUT);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(4,HIGH);
  int Joy_x = analogRead(x);
  if (Joy_x < 500) {
    Serial.println("Left");
    digitalWrite(left, HIGH);
    digitalWrite(right, LOW);
  }
  else if (Joy_x > 600) {
    Serial.println("Right");
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
  } else {
    Serial.println("Center");
    digitalWrite(left, LOW);
    digitalWrite(right, LOW);
  }

}

