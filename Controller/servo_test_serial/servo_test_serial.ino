#include <Servo.h>

Servo myservo;
int val=0;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
}

void loop() {
  while (Serial.available())
  {
    delay(10);
    val = Serial.readString().toInt();
    myservo.write(val);
    Serial.println(String(val) + " ok");
    delay(100);
  }
}
