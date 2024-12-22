void setup() {
  Serial.begin(9600);
}

void loop() {
  int potM = analogRead(A2);
  Serial.print("Value: ");
  Serial.println(map(potM, 0, 1023, 0, 255));
  delay(100);
}
