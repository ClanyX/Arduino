const int sensorPIN = 3;

void setup() {
  pinMode(sensorPIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  int read = digitalRead(sensorPIN);
  Serial.println(read);
}
