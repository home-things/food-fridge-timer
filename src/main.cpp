#include <Arduino.h>
#include <LiquidCrystal.h>

//                rs  en  d0 d1 d2 d3
LiquidCrystal lcd(12, 13, 4, 5, 6, 7);
#define PERIOD_PIN A1
#define VIBRO_PIN 9
unsigned prevPeriod = 0;
unsigned minPeriod = 2, periodSteps = 4; // 1, 2, 3, 4, 5


// Creat a set of new characters
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

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
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

void setup()
{
  Serial.begin ( 57600 );
  pinMode(PERIOD_PIN, INPUT);
  pinMode(VIBRO_PIN, OUTPUT);

  lcd.begin(16,2);               // initialize the lcd

  // lcd.createChar (0, smiley);    // load character to the LCD
  // lcd.createChar (1, armsUp);    // load character to the LCD
  // lcd.createChar (2, frownie);   // load character to the LCD

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
    delay(100);
    digitalWrite(VIBRO_PIN, LOW);
  }

  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print(period); // 1 .. 5

  prevPeriod = period;

  delay(200);

  // // Do a little animation by writing to the same location
  // lcd.setCursor ( 14, 1 );
  // lcd.print (char(2));
  // delay (200);
  // lcd.setCursor ( 14, 1 );
  // lcd.print ( char(0));
  // delay (200);
}
