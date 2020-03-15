#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

//                rs  en  d0 d1 d2 d3
LiquidCrystal lcd(12, 13, 4, 5, 6, 7);

//
// pins
#define PERIOD_PIN A1
#define VIBRO_PIN  9
#define BUTTON_PIN 2

//
// eeprom memory addresses
#define PERIOD_MEM 0
#define STARTED_AT 1

//
// consts
#define MAX_PERIOD 5

byte prevPeriod = 0;
byte minPeriod = 2, periodSteps = 4; // 1, 2, 3, 4, 5
byte period; // scaleLen
byte iterationIndex = false;
unsigned long lastBtnPressAt = 0;

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

byte topRoundArrow[8] = {
	0b11111,
	0b00001,
	0b01101,
	0b11101,
	0b11001,
	0b10000,
	0b10000,
	0b10000,
};

byte bottomRoundArrow[8] = {
	0b00001,
	0b00001,
	0b00001,
	0b10011,
	0b10111,
	0b10110,
	0b10000,
	0b11111,
};

void printScale (byte day, byte period) {
  lcd.setCursor(0, 1);
  for (byte i = 0; i < day; ++i) {
    lcd.print("\4\4\3");
  }
  for (byte i = day; i < period; ++i) {
    lcd.print("\3\3\3");
  }
  for (byte i = period; i < MAX_PERIOD; ++i) {
    lcd.print("   ");
  }
  lcd.setCursor(14, 1);
  lcd.print(" "); // clear trailing \3
}

void vibroBeep () {
  digitalWrite(VIBRO_PIN, HIGH);
  for(size_t i = 0; i < 2e4; ++i);
  digitalWrite(VIBRO_PIN, LOW);
}

void handlePress () {
  if (millis() - lastBtnPressAt < 300) return;
  lastBtnPressAt = millis();

  if (state == WASTED || state == PENDING) {
    state = INITIAL;
  } else {
    state = (STATE)(state + 1);
  }
  vibroBeep();
}

void setup()
{
  Serial.begin (57600);

  pinMode(PERIOD_PIN, INPUT);
  pinMode(VIBRO_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  lcd.begin(16,2);
  lcd.home();

  lcd.createChar (3, newbar);
  lcd.createChar (4, fullbar);
  lcd.createChar (5, topRoundArrow);
  lcd.createChar (6, bottomRoundArrow);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handlePress, RISING);
  
  state = INITIAL;
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
        EEPROM.write(PERIOD_MEM, period);
        vibroBeep();
      }
      printScale(0, period);
      lcd.print(period);

      prevPeriod = period;
      break;
    }
    case PENDING: {
      byte day = 0;
      printScale(day, period);
      lcd.print(day);
      // EEPROM.write(STARTED_AT, time());
      lcd.setCursor(0, 0);
      lcd.print(iterationIndex >= 5 ? "*" : " ");
      lcd.setCursor(15, 0);
      lcd.print(iterationIndex >= 5 ? "*" : " ");

      iterationIndex = iterationIndex == 9 ? 0 : 1 + iterationIndex;

      break;
    }
    case WASTED: {
      break;
    }
  } while (false);

  delay(200);
}
