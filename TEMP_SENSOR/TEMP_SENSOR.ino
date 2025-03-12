#define firstTemp A0;
#define secondTemp A1;
#define thirdTemp A2;

float temp;

void setup() {
  Serial.begin(9600);
}

void loop() {
  float vol1 = analogRead(A0) * (5.0 / 1024.0);
  float vol2 = analogRead(A1) * (5.0 / 1024.0);
  float vol3 = analogRead(A2) * (5.0 / 1024.0);
  temp = (vol1 + vol2 + vol3) / 3.0;
  temp = (temp - 0.5) * 100;
  Serial.print("Temperature: ");
  Serial.println(temp);
  delay(1000);
}
