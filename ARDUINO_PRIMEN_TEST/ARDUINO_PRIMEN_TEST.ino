int number = 0;
int primeCount = 0;

bool isPrime(int n) {
  if(n <= 1) return false;
  for (int i = 2; i * i <= n; i++)
  {
    if(n % i == 0) return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("How many prime can Arduino find in 20 seconds.");
  Serial.println("Press any key to start.");
  while(!Serial.available());
  Serial.read();

  Serial.println("Start");
  unsigned long start = millis();
  while (millis() - start < 20000)
  {
    if(isPrime(number))
      primeCount++;
    number++;
  }
  Serial.println("End");
  Serial.println("Prime count: " + String(primeCount));
}

void loop() {
}