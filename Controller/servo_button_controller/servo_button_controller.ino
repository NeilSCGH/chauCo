#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
  myservo.write(0);
  //delay(3000);
}

void loop() {
  myservo.write(0);
  delay(2000);
  myservo.write(75);
  delay(2000);
}
