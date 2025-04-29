void initTimer(){
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);

  OCR1A = 31249; //2s

  TIMSK1 = (1 << OCIE1A);

  interrupts();
}

volatile unsigned long ms = 0;

ISR(TIMER1_COMPA_vect){
  ms++;
}

void setup() {
  initTimer();
  pinMode(13, OUTPUT);
}

void loop() {
  static unsigned long oldBlink = 0;
  if(ms - oldBlink >= 0.5){
    oldBlink = ms;
    digitalWrite(13, !digitalRead(13));
  }
}
