///libraries
#include "U8glib.h"
#include "avr/sleep.h"
#include "avr/power.h"
#include "avr/wdt.h"

///define pines
#define humiditySensor A0
#define pump 5

///variables
U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);
int humiditySensorData = 0;
volatile int f_wdt = 0;
int cicleCounter = 0;

int dryLevel = 500;   //when the sensor checked
int pumpSpeed = 200;  //level of analog signal to pump
int pumpTime = 5;     //seconds
//  int interuptCounter = 150;  //8 second times 150 = 20 min

//just for test
int interuptCounter = 2;

///interupt
ISR(WDT_vect) {
  if (f_wdt == 0) {
    f_wdt = 1;
  }
}

///functions
void testDirt() {
  humiditySensorData = analogRead(humiditySensor);

  //delete
  Serial.println(cicleCounter);
  //

  //check the data - dry level
  if (humiditySensorData > dryLevel) {
    //display
    display.firstPage();
    do {
      displayArt();
    } while (display.nextPage());

    //motor
    motorRun();
  }
}

void enterSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  //Here will program continue after the interupt stack 150 times

  sleep_disable();
  power_all_enable();
}

void setupInterupt() {
  MCUSR &= ~(1 << WDRF);

  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0);
  WDTCSR |= _BV(WDIE);
}

void motorRun() {
  analogWrite(pump, pumpSpeed);
  delay(pumpTime * 1000);
  analogWrite(pump, 0);
}

void displayArt() {
  display.setFont(u8g_font_unifont);
  display.setPrintPos(0, 10);
  display.print(millis() / 1000);
}

void setup() {
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  setupInterupt();

  //delete
  Serial.begin(9600);
  //
}

void loop() {
  if (f_wdt != 1) return;
  cicleCounter++;
  if (cicleCounter == interuptCounter) {
    testDirt();
    cicleCounter = 0;
    enterSleep();
  }
  f_wdt = 0;
}