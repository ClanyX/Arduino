#define interuptPin 2
#define led 9
#define builtIn 13

void setup() {
  pinMode(led, OUTPUT);
  pinMode(builtIn, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interuptPin), detectBtn, RISING);
}

void loop() {
  digitalWrite(builtIn, !digitalRead(builtIn));
  delay(500);
}

void detectBtn() {
  digitalWrite(led, !digitalRead(led));
}
