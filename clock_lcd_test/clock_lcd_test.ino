#include <DS3231.h>
DS3231 myRTC;

#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() { 
  // Start the I2C interface
  Wire.begin();
 
  // Start the serial interface
  Serial.begin(57600);

  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
}

void loop() { 
// lcd.setCursor(0,0);
// lcd.print("Hour:  ");
// lcd.print(myRTC.getHour());
 
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(myRTC.getDate());
 
 delay(1000); 
}
