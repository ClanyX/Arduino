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
volatile bool f_wdt = false;
volatile byte cycleCount = 0;

int dryLevel = 500;   //when the sensor checked
int pumpSpeed = 200;  //level of analog signal to pump
int pumpTime = 5;     //seconds
int interuptCounter = 150;  //8 second times 150 = 20 min

//just for test
//int interuptCounter = 5;

///interupt
ISR(WDT_vect) {
  cycleCount++;
  if (cycleCount >= interuptCounter) {
    f_wdt = true;
    cycleCount = 0;
  }
}

///functions
void testDirt() {
  humiditySensorData = analogRead(humiditySensor);

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
  sleep_mode();

  //Here will program continue after the interupt stack 150 times

  power_all_enable();
}

void setupWatchDog() {
  cli();
  wdt_reset();
  WDTCSR = (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);
  sei();
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

  setupWatchDog();
}

void loop() {
  enterSleep();

  if (f_wdt) {
    f_wdt = false;
    testDirt();
  }
}