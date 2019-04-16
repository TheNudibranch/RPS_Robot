// The pins that have been tested and are availabl to control the servo are:
// [10,9,6,5,3]

#include <Servo.h>

Servo myServo;

int const potPin = A0;
int potVal;
int angle;

void setup() {
  myServo.attach(3); // assign pin

  Serial.begin(9600);
}

void loop() {
  potVal = analogRead(potPin);
  Serial.print("Pot Value: ");
  Serial.print(potVal);

  angle = map(potVal, 0, 1023, 0, 160);
  Serial.print(", Angle: ");
  Serial.println(angle);

  myServo.write(angle);
  delay(15);
}
