///libraries
#include "U8glib.h"
#include "avr/sleep.h"
#include "avr/power.h"
#include "avr/wdt.h"

///define pines
#define humiditySensor A0
#define pump 5

///define variables
#define dryLevel = 500         //when the sensor checked
#define pumpSpeed = 200        //level of analog signal to pump
#define pumpTime = 5           //seconds
#define interuptCounter = 150  //8 second times 150 = 20 min

///variables
U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);
int humiditySensorData = 0;
bool animationPick = true;
volatile bool f_wdt = false;
volatile byte cycleCount = 0;

int sunState = 0;
int cloudState = 0;

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
    //motor
    motorRun();

    //display
    display.firstPage();
    do {
      displayArt();
    } while (display.nextPage());
  } else {
    if (animationPick) {
      drawSun(sunState);
      animationPick = !animationPick;
    } else {
      drawCloud(cloudState);
      animationPick = !animationPick;
    }
  }
}

//animation drink
int drinkState = 0;

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
  u8g.drawLine(x1, y1, x2, y2);
}

void drawDrinkState(int state) {
  u8g.drawRFrame(44, 20, 40, 40, 5);  // Tělo skleničky
  u8g.drawBox(48, 18, 32, 2);         // Víčko
  u8g.drawHLine(46, 62, 36);          // Stín

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

  u8g.drawBox(44, drinkLevelY, 40, drinkLevelHeight);

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
    u8g.drawCircle(55, bubbleY1, 1);
    u8g.drawCircle(68, bubbleY2, 1);
    u8g.drawCircle(60, bubbleY3, 1);
    u8g.drawCircle(73, bubbleY4, 1);
  }

  animationState++;
  if (animationState > 4) {
    animationState = 0;
  } else {
    delay(625);
    drawDrinkState(state);
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
  u8g.drawDisc(centerX, centerY, radius);

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
  u8g.drawLine(centerX - radius - offset, centerY, centerX - radius - offset - rayLength, centerY);  // Vlevo
  u8g.drawLine(centerX + radius + offset, centerY, centerX + radius + offset + rayLength, centerY);  // Vpravo
  u8g.drawLine(centerX, centerY - radius - offset, centerX, centerY - radius - offset - rayLength);  // Nahoru
  u8g.drawLine(centerX, centerY + radius + offset, centerX, centerY + radius + offset + rayLength);  // Dolů

  // Úhlopříčné paprsky
  int diagX = (radius * 0.707);
  int diagY = (radius * 0.707);

  u8g.drawLine(centerX - diagX - offset, centerY - diagY - offset, centerX - diagX - offset - rayLength, centerY - diagY - offset - rayLength);  // Horní levý
  u8g.drawLine(centerX + diagX + offset, centerY - diagY - offset, centerX + diagX + offset + rayLength, centerY - diagY - offset - rayLength);  // Horní pravý
  u8g.drawLine(centerX - diagX - offset, centerY + diagY + offset, centerX - diagX - offset - rayLength, centerY + diagY + offset + rayLength);  // Dolní levý
  u8g.drawLine(centerX + diagX + offset, centerY + diagY + offset, centerX + diagX + offset + rayLength, centerY + diagY + offset + rayLength);  // Dolní pravý

  animationState++;
  if (animationState > 4) {
    animationState = 0;
  } else {
    delay(200);
    drawSun(state);
  }
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
  u8g.drawDisc(cloudX, cloudY + cloudOffsetY, 15);
  u8g.drawDisc(cloudX + 20, cloudY + 5 + cloudOffsetY, 12);
  u8g.drawDisc(cloudX + 40, cloudY + cloudOffsetY, 15);
  u8g.drawDisc(cloudX + 20, cloudY - 10 + cloudOffsetY, 10);

  animationState++;
  if (animationState > 3) {
    animationState = 0;
  } else {
    delay(250);
    drawCloud(state);
  }
}
//

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
  drawDrinkState(drinkState);

  display.setFont(u8g_font_unifont);
  display.drawStr(0, 20, humiditySensorData);
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