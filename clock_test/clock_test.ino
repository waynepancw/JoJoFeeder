#include <DS3231.h>
#include <Wire.h>
#include <time.h>

DS3231 myRTC;
bool h12Flag;
bool pmFlag;

void setup() {
  Serial.begin(57600); 
}

void loop()
{
  
  // then the date
  Serial.print(myRTC.getDate());
  Serial.print(" ");
  
  // and the day of the week
  Serial.print(myRTC.getDoW());
  Serial.print(" ");
  
  // Finally the hour, minute, and second
  Serial.print(myRTC.getHour(h12Flag, pmFlag));
  Serial.print(" ");
  Serial.print(myRTC.getMinute());
  Serial.print(" ");
  Serial.println(myRTC.getSecond());
  
  // Wait one second before repeating
  delay (1000);
}
