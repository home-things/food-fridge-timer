#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>

//                rs  en  d0 d1 d2 d3
LiquidCrystal lcd(12, 13, 4, 5, 6, 7);

//
// pins
#define PERIOD_PIN A1
#define VIBRO_PIN  9
#define BUTTON_PIN 2

//
// consts
#define MAX_PERIOD 5

unsigned prevPeriod = 0;
unsigned minPeriod = 2, periodSteps = 4; // 1, 2, 3, 4, 5
unsigned period; // scaleLen

enum STATE {
  WELCOME,
  INITIAL,
  SETTING,
  PENDING,
  WASTED
};
auto state = WELCOME;

char label[5][17] = {
  "   loading...",
  "FridgeFood Guard",   // 2: Setup timer [ok]
  " Turn  wheel \5\6 ", // 2: Exp time, days
  " Timer  pending ",   // 2: Days left // Eat me, pls
  "PO  TRA  CHE  NO"
};

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

byte topRoundArrow[8] = 
{
	0b11111,
	0b00001,
	0b01101,
	0b11101,
	0b11001,
	0b10000,
	0b10000,
	0b10000,
};

byte bottomRoundArrow[8] = 
{
	0b00001,
	0b00001,
	0b00001,
	0b10011,
	0b10111,
	0b10110,
	0b10000,
	0b11111,
};

void printScale (unsigned day, unsigned period) {
  lcd.setCursor(0, 1);
  for (unsigned i = 0; i < day; ++i) {
    lcd.print("\4\4\3");
  }
  for (unsigned i = day; i < period; ++i) {
    lcd.print("\3\3\3");
  }
  for (unsigned i = period; i < MAX_PERIOD; ++i) {
    lcd.print("   ");
  }
  lcd.setCursor(14, 1);
  lcd.print(" "); // clear trailing \3
}

void handlePress () {
  if (state == WASTED || state == PENDING) {
    state = INITIAL;
  } else {
    state = (STATE)(state + 1);
  }
}

void setup()
{
  Serial.begin (57600);

  pinMode(PERIOD_PIN, INPUT);
  pinMode(VIBRO_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.begin(16,2);
  lcd.home();

  lcd.createChar (3, newbar);
  lcd.createChar (4, fullbar);
  lcd.createChar (5, topRoundArrow);
  lcd.createChar (6, bottomRoundArrow);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handlePress, FALLING);
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print(label[state]);

  do switch (state) {
    case INITIAL: {
      lcd.setCursor(0, 1);
      lcd.print("Setup timer [ok]");
      break;
    }
    case SETTING: {
      unsigned raw = 0;
      raw = analogRead(PERIOD_PIN); // 0 .. 1023 
      
      period = minPeriod + periodSteps * (1 - ((1 + raw) / 1024.0));
      
      if (period != prevPeriod) {
        digitalWrite(VIBRO_PIN, HIGH);
        delay(133);
        digitalWrite(VIBRO_PIN, LOW);
      }
      printScale(0, period);
      lcd.print(period);

      prevPeriod = period;
      break;
    }
    case PENDING: {
      unsigned day = 0;
      printScale(day, period);
      lcd.print(day);

      break;
    }
    case WASTED: {
      break;
    }
  } while (false);

  delay(200);
}
