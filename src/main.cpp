#include <Arduino.h>
#include <LiquidCrystal.h>

//                rs  en  d0 d1 d2 d3
LiquidCrystal lcd(12, 13, 4, 5, 6, 7);

//
// pins
#define PERIOD_PIN A1
#define VIBRO_PIN 9

//
// consts
#define MAX_PERIOD 5

unsigned prevPeriod = 0;
unsigned minPeriod = 2, periodSteps = 4; // 1, 2, 3, 4, 5
unsigned scaleLen = 5;

//
// sumbols

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

// a bar
byte fullbar[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// track
byte newbar[8] = {
 0b01001,
 0b10010,
 0b00100,
 0b01001,
 0b10010,
 0b00100,
 0b01001,
 0b10010,
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

void printScale (unsigned period, unsigned len) {
  lcd.setCursor(0, 1);
  for (unsigned i = 0; i < period; ++i) {
    lcd.print("\4\4\3");
  }
  for (unsigned i = period; i < len; ++i) {
    lcd.print("\3\3\3");
  }
  for (unsigned i = len; i < MAX_PERIOD; ++i) {
    lcd.print("   ");
  }
  lcd.setCursor(14, 1);
  lcd.print(" "); // clear trailing \3
  lcd.print(period ? period : len);
}

void setup()
{
  Serial.begin ( 57600 );
  pinMode(PERIOD_PIN, INPUT);
  pinMode(VIBRO_PIN, OUTPUT);

  lcd.begin(16,2);               // initialize the lcd

  // lcd.createChar (0, smiley);    // load character to the LCD
  // lcd.createChar (1, armsUp);    // load character to the LCD
  // lcd.createChar (2, frownie);   // load character to the LCD
  lcd.createChar (3, newbar);
  lcd.createChar (4, fullbar);

  lcd.home();                   // go home
  lcd.print("FridgeFood timer");
  lcd.setCursor ( 0, 1 );        // go to the next line
  // lcd.print ("  press  start  ");
  lcd.setCursor ( 0, 1 );        // go to the next line
  // lcd.cursor();
  // lcd.blink();

}

void loop()
{
  unsigned raw = 0;
  raw = analogRead(PERIOD_PIN); // 0 .. 1023 
  
  unsigned period = minPeriod + periodSteps * (1 - ((1 + raw) / 1024.0));
  
  if (period != prevPeriod) {
    digitalWrite(VIBRO_PIN, HIGH);
    delay(133);
    digitalWrite(VIBRO_PIN, LOW);
  }

  // lcd.setCursor(0, 1);
  // lcd.print("     ");
  // lcd.setCursor(0, 1);
  // lcd.print(period); // 1 .. 5

  // lcd.setCursor(0,1);
  // lcd.print("\4\4\3\4\4\3\4\4\3\4\4\3\4\4 ");
  // lcd.print(period);
  // printScale(period, scaleLen);
  // lcd.print(period);
  printScale(0, period);

  prevPeriod = period;

  delay(200);
}
