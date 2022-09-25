#include <Wire.h>
#include <time.h>

#include <Servo.h>
Servo myServo;  

#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal display(rs, en, d4, d5, d6, d7);


#include <DS3231.h> 
DS3231 myRTC;
byte t

#include <EEPROM.h>

//Inputs/Outputs
int but1 = 2;
int but2 = 5;
int but3 = 4;
int servo_pin = 9;
int Buzzer = 6;

//Variables
bool state_but1 = true;
bool state_but2 = true;
bool state_but3 = true;
int stop_angle = 92;                //Please test values around 90 till the motor is not rotating anymore
int rotate_angle = 180;
int portions = 5;
int interval = 8;
unsigned int millis_before = 0;     //We use these to create the loop refresh rate
unsigned int millis_now = 0;
int OLED_refresh_rate = 200;
int max_portions = 20;
int min_portions = 3;
int Hour, Minute;
int last_feed_hour = 1;
int next_feed_hour = 1;
bool feed_active = true;
int portion_delay = 500;


void setup() {
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  myServo.attach(servo_pin);  
  myServo.write(stop_angle);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  Serial.begin(9600);

//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
//  delay(100);
//  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.display();

//  myRTC.begin();
  //myRTC.setDOW(TUESDAY);            // Set Day-of-Week to SUNDAY
  //myRTC.setTime(18, 03, 0);         // Set the time to 12:00:00 (24hr format)
  //myRTC.setDate(2, 11, 2021);       // Set the date to January 1st, 2014
 
  
  last_feed_hour = EEPROM.read(2);  //We store the last feeding hour on address 2
  next_feed_hour = EEPROM.read(3);  //We store the next feeding hour on address 3
  
  millis_before = millis();
  millis_now = millis();
}

void loop() {  
  millis_now = millis();                                  //Get actual millis values each loop
  if(millis_now - millis_before > OLED_refresh_rate){     //Refresh rate of the OLED
    millis_before = millis();                             //Save "before value" for next loop
    display.clear();   
    display.setCursor(0,0);    
    display.print(portions); 
    display.print(" portions"); 
   
    display.setCursor(90,9);
    display.print(Hour); 
    display.print(":"); 
    display.print(Minute); 
   
    display.setCursor(0,16);
    display.print("Each "); 
    display.print(interval); 
    display.print(" hours"); 
    display.display(); 
  }


  t = myRTC.getTime();                          //Get time from DS3232
  Hour = t.hour;                              //Get the hour in 0h-23h format
  Minute = t.min;                             //Get minute as well just in case
  Serial.println(Hour);                       //Print hour for debug

  if(Hour == next_feed_hour){                 //If the time is the "next_feed_hour",we activate feeding
    feed_active = true;
    last_feed_hour = Hour;
    next_feed_hour = Hour + interval;         //Increase next feeding time by the interval
    if(next_feed_hour >= 23){                 //if we pass 23 hours, we start back at 00 hours
      next_feed_hour = next_feed_hour - 24;   //That's why we substract 24 hours which is maximum
    }
    EEPROM.write(2, last_feed_hour);          //Write on memory when was the last feeding
    EEPROM.write(3, next_feed_hour);          //Also when will be the next feeding process
    analogWrite(Buzzer, 200);
    delay(200);
    digitalWrite(Buzzer, LOW);
  }

 
  if(feed_active){                            //If this is activem we rotate the motor
    int i = 0;
    while(i<portions){                        //Rotate the motor according to the portions value
      myServo.write(rotate_angle);          //START the motor
      i++;
      display.clear();   
      display.setCursor(48,11);      
      display.print("FEEDING");    
      display.display();      
      delay(portion_delay);                   //Delay for each portion is milliseconds
    }
    myServo.write(stop_angle);              //STOP the motor
    feed_active = false;                      //Set "feed_active" back to false
  }

  /////////////////////////////////
  //Button1 (increase portions)   
  if(!digitalRead(but1) && state_but1){
    portions++;
    if(portions > max_portions){          //Where "max_portions" is set above
      portions = min_portions;
    }
    state_but1 = false;
  }
  else if(digitalRead(but1) && !state_but1){
    state_but1 = true;
  }
  
  /////////////////////////////////
  //Button2 (Manual feed) 
  if(!digitalRead(but2) && state_but2){    
    EEPROM.write(2, Hour);
    
    next_feed_hour = Hour + interval;
    if(next_feed_hour >= 23){
      next_feed_hour = next_feed_hour - 24;
    }    
    EEPROM.write(3, next_feed_hour);
    
    feed_active = true;
    state_but2 = false;
  }
  else if(digitalRead(but2) && !state_but2){
    state_but2 = true;
  }

  /////////////////////////////////
  //Button3 (increase time inteval 0h.23h)
  if(!digitalRead(but3) && state_but3){
    interval++;
    if(interval > 23){
      interval = 1;
    }
    state_but3 = false;
  }
  else if(digitalRead(but3) && !state_but3){
    state_but3 = true;
  }  
}//End of void loop
