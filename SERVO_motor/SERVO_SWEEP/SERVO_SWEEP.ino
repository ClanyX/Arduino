#include <Servo.h>
Servo servo;

int pos = 0;

void setup() {
  servo.attach(9);
}

void loop() {
  for(pos = 0; pos <= 180; pos+=1){
    servo.write(pos);
    delay(15);
  }

  for(pos = 180; pos >= 0; pos-=1){
    servo.write(pos);
    delay(15);
  }

  play();
  delay(5000);
}

void play(){
    for(pos = 0; pos <= 45; pos+=1){
    servo.write(pos);
    delay(3);
  }
  servo.write(90);
}
