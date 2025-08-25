///libraries
#include "U8glib.h"
#include "avr/sleep.h"
#include "avr/power.h"
#include "avr/wdt.h"

///define pines
#define humiditySensor A0
#define pump 5

///define variables
#define dryLevel 530       //when the sensor checked
#define pumpTime 5         //seconds
#define interuptCounter 2 //8 second times 150 = 20 min

///variables
U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);
int humiditySensorData = 0;
bool animationPick = true;
volatile bool f_wdt = false;
volatile byte cycleCount = 0;

int sunState = 0;
int cloudState = 0;
int drinkState = 0;

///interupt
ISR(WDT_vect) {
  cycleCount++;
  if (cycleCount >= interuptCounter) {
    f_wdt = true;
    cycleCount = 0;
  }
}

//animation drink
void drawStraw(int state) {
  int x1 = 70;
  int y1 = 20;
  int x2 = 75;
  int y2 = 5;

  // Brčko se hýbe jen ve stavech 0 až 3
  if (state < 4) {
    switch (state) {
      case 1:
        x1 = 68;
        x2 = 73;
        break;
      case 3:
        x1 = 72;
        x2 = 77;
        break;
    }
  }
  display.drawLine(x1, y1, x2, y2);
}

void drawDrinkState(int state) {
  display.drawRFrame(44, 20, 40, 40, 5);  // Tělo skleničky
  display.drawBox(48, 18, 32, 2);         // Víčko
  display.drawHLine(46, 62, 36);          // Stín

  // Vykreslení animovaného brčka
  drawStraw(state);

  // Animace hladiny pití
  int drinkLevelHeight = 0;
  int drinkLevelY = 0;

  switch (state) {
    case 0:  // Plná sklenička
      drinkLevelHeight = 35;
      drinkLevelY = 25;
      break;
    case 1:
      drinkLevelHeight = 30;
      drinkLevelY = 30;
      break;
    case 2:
      drinkLevelHeight = 25;
      drinkLevelY = 35;
      break;
    case 3:
      drinkLevelHeight = 20;
      drinkLevelY = 40;
      break;
    case 4:  // Sklenička je prázdná
      drinkLevelHeight = 10;
      drinkLevelY = 50;
      break;
  }

  display.drawBox(44, drinkLevelY, 40, drinkLevelHeight);

  // Animace bublinek
  int bubbleY1 = 0;
  int bubbleY2 = 0;
  int bubbleY3 = 0;
  int bubbleY4 = 0;

  switch (state) {
    case 0:
      bubbleY1 = 50;
      bubbleY2 = 55;
      bubbleY3 = 40;
      bubbleY4 = 45;
      break;
    case 1:
      bubbleY1 = 45;
      bubbleY2 = 50;
      bubbleY3 = 35;
      bubbleY4 = 40;
      break;
    case 2:
      bubbleY1 = 40;
      bubbleY2 = 45;
      bubbleY3 = 30;
      bubbleY4 = 35;
      break;
    case 3:
      bubbleY1 = 35;
      bubbleY2 = 40;
      bubbleY3 = 25;
      bubbleY4 = 30;
      break;
    case 4:  // Bublinky zmizí
      break;
  }

  if (state < 4) {
    display.drawCircle(55, bubbleY1, 1);
    display.drawCircle(68, bubbleY2, 1);
    display.drawCircle(60, bubbleY3, 1);
    display.drawCircle(73, bubbleY4, 1);
  }
}
//

//animation sun
void drawSun(int state) {
  // Střed sluníčka
  int centerX = 64;
  int centerY = 32;
  int radius = 15;

  // Tělo sluníčka (plný kruh)
  display.drawDisc(centerX, centerY, radius);

  // Délka paprsků
  int rayLength = 5;

  // Pozice paprsků se mění na základě stavu animace
  int offset;

  switch (state) {
    case 0:
      offset = 0;  // Paprsky u sluníčka
      break;
    case 1:
      offset = 3;
      break;
    case 2:
      offset = 6;
      break;
    case 3:
      offset = 3;
      break;
    case 4:
      offset = 0;  // Paprsky u sluníčka
      break;
  }

  // Svislé a vodorovné paprsky
  display.drawLine(centerX - radius - offset, centerY, centerX - radius - offset - rayLength, centerY);  // Vlevo
  display.drawLine(centerX + radius + offset, centerY, centerX + radius + offset + rayLength, centerY);  // Vpravo
  display.drawLine(centerX, centerY - radius - offset, centerX, centerY - radius - offset - rayLength);  // Nahoru
  display.drawLine(centerX, centerY + radius + offset, centerX, centerY + radius + offset + rayLength);  // Dolů

  // Úhlopříčné paprsky
  int diagX = (radius * 0.707);
  int diagY = (radius * 0.707);

  display.drawLine(centerX - diagX - offset, centerY - diagY - offset, centerX - diagX - offset - rayLength, centerY - diagY - offset - rayLength);  // Horní levý
  display.drawLine(centerX + diagX + offset, centerY - diagY - offset, centerX + diagX + offset + rayLength, centerY - diagY - offset - rayLength);  // Horní pravý
  display.drawLine(centerX - diagX - offset, centerY + diagY + offset, centerX - diagX - offset - rayLength, centerY + diagY + offset + rayLength);  // Dolní levý
  display.drawLine(centerX + diagX + offset, centerY + diagY + offset, centerX + diagX + offset + rayLength, centerY + diagY + offset + rayLength);  // Dolní pravý
}
//

//animation cloud
void drawCloud(int state) {
  // Základní pozice mráčku (uprostřed obrazovky)
  int cloudX = 45;
  int cloudY = 32;

  // Pohyb mráčku nahoru a dolů (od 0 do 4 pixelů)
  int cloudOffsetY;

  switch (state) {
    case 0:
      cloudOffsetY = 0;  // Výchozí pozice
      break;
    case 1:
      cloudOffsetY = -2;  // Posun nahoru
      break;
    case 2:
      cloudOffsetY = 0;  // Návrat do středu
      break;
    case 3:
      cloudOffsetY = 2;  // Posun dolů
      break;
  }

  // Vykreslení mráčku (čtyři kruhy, které se překrývají)
  display.drawDisc(cloudX, cloudY + cloudOffsetY, 15);
  display.drawDisc(cloudX + 20, cloudY + 5 + cloudOffsetY, 12);
  display.drawDisc(cloudX + 40, cloudY + cloudOffsetY, 15);
  display.drawDisc(cloudX + 20, cloudY - 10 + cloudOffsetY, 10);
}
//

///functions
void testDirt() {
  humiditySensorData = analogRead(humiditySensor);

  //check the data - dry level
  if (humiditySensorData > dryLevel) {
    //motor
    motorRun();

    //display
    while (drinkState < 5) {
      display.firstPage();
      do {
        drawDrinkState(drinkState);
      } while (display.nextPage());
      drinkState++;
      delay(625);
    }
    drinkState = 0;
    display.firstPage();
    do {
    } while (display.nextPage());
  } else {
    if (animationPick) {
      while (sunState < 5) {
        display.firstPage();
        do {
          drawSun(sunState);
        } while (display.nextPage());
        sunState++;
        delay(200);
      }
      sunState = 0;
      display.firstPage();
      do {
      } while (display.nextPage());

      animationPick = !animationPick;
    } else {
      while (cloudState < 4) {
        display.firstPage();
        do {
          drawCloud(cloudState);
        } while (display.nextPage());
        cloudState++;
        delay(250);
      }
      cloudState = 0;
      display.firstPage();
      do {
      } while (display.nextPage());
      animationPick = !animationPick;
    }
  }
}

void enterSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();

  //Here will program continue after the interupt stack 150 times

  power_all_enable();
  delay(100);
}

void setupWatchDog() {
  cli();
  wdt_reset();
  WDTCSR = (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);
  sei();
}

void motorRun() {
  digitalWrite(pump, HIGH);
  delay(pumpTime * 1000);
  digitalWrite(pump, LOW);
}

void setup() {
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  display.begin();
  setupWatchDog();
}

void loop() {
  enterSleep();

  if (f_wdt) {
    f_wdt = false;
    testDirt();
  }
}