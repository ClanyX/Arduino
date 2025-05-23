void initTimer() {
  noInterrupts();

  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);

  //16MHz / 64 / (x + 1) = 2kHz
  OCR0A = 124; //500ms

  TIMSK0 = (1 << OCIE0A);

  interrupts();
}

volatile unsigned long ms = 0;

ISR(TIMER0_COMPA_vect){
  ms++;
}

void setup() {
  initTimer();
  pinMode(13, OUTPUT);
}

void loop() {
  static unsigned long lastBlink = 0;
  if(ms - lastBlink >= 500){
    lastBlink = ms;
    digitalWrite(13, !digitalRead(13));
  }
}
