#include <Arduino.h>
#include <LiquidCrystal.h>

#define   CONTRAST_PIN   A2
#define   BACKLIGHT_PIN  A3
#define   CONTRAST       110
//                rs  en  d0 d1 d2 d3 backlight
LiquidCrystal lcd(12, 13, 4, 5, 6, 7);


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

  // Switch on the backlight and LCD contrast levels
  pinMode(BACKLIGHT_PIN, OUTPUT);
  pinMode(CONTRAST_PIN, OUTPUT);

  analogWrite(BACKLIGHT_PIN, (3.3 / 5) * 255);
  analogWrite(CONTRAST_PIN, CONTRAST);

  //lcd.setBacklightPin ( BACKLIGHT_PIN, POSITIVE );
  //lcd.setBacklight ( HIGH );
  lcd.backlight();

  lcd.begin(16,2);               // initialize the lcd

  lcd.createChar (0, smiley);    // load character to the LCD
  lcd.createChar (1, armsUp);    // load character to the LCD
  lcd.createChar (2, frownie);   // load character to the LCD

  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" FORUM - fm   ");
}

void loop()
{
  // Do a little animation by writing to the same location
  lcd.setCursor ( 14, 1 );
  lcd.print (char(2));
  delay (200);
  lcd.setCursor ( 14, 1 );
  lcd.print ( char(0));
  delay (200);
}
