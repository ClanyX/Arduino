#include <Servo.h>
Servo servo;

const int potpin = 0;
int val;

void setup() {
  servo.attach(9);
}

void loop() {
  val = analogRead(potpin);
  val = map(val, 0, 1023, 0, 180);
  servo.write(val);
  delay(15);
}
