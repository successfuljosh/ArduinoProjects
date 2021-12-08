
#define enA 5
#define enB 10
#define in1 6
#define in2 7
#define in3 8
#define in4 9

void setup() {
  // put your setup code here, to run once:
pinMode(enA,OUTPUT);
pinMode(enB,OUTPUT);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(in3,OUTPUT);
pinMode(in4,OUTPUT);
Serial.begin(9600);


digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in3,HIGH);
digitalWrite(in4,LOW);


}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(enA,HIGH);
digitalWrite(enB,HIGH);

digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in3,HIGH);
digitalWrite(in4,LOW);

Serial.println("forward");
delay(10000);

pinMode(in1,LOW);
pinMode(in2,LOW);
pinMode(in3,LOW);
pinMode(in4,LOW);

Serial.println("Stop");
delay(3000);



}
