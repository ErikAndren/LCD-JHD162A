/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Serial.h>

#define NEW_LINE_THRESHOLD 1000
#define LCD_COLUMNS 16
#define LCD_ROWS     2

int charCnt = 0;
unsigned long timeStamp = 0;
char lineBuf[LCD_COLUMNS];

// initialize the library with the numbers of the interface pins  
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.clear();
  
  timeStamp = millis();

  Serial.begin(9600);

  memset(lineBuf, 0, sizeof(lineBuf));
}

void migrateLine() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  //Move the old second row to the first
  for (int i = 0; i < LCD_COLUMNS; i++) {
    if (lineBuf[i] == 0) {
        break;
    }
    lcd.write(lineBuf[i]); 
  }
  memset(lineBuf, 0, sizeof(lineBuf));
           
  charCnt = 0;
  lcd.setCursor(0, 1);  
}

void loop() {
  if (Serial.available() > 0) {
    unsigned long newTimeStamp = millis();

    //Migrate the old second line to the first line
    if (charCnt >= LCD_COLUMNS) {
      migrateLine();
    } else {
      //Try to detect the end of a line
      unsigned long delta = newTimeStamp - timeStamp;
      if (delta > NEW_LINE_THRESHOLD) {
        migrateLine();        
      }
    }
 
    lineBuf[charCnt] = Serial.read();    

    timeStamp = newTimeStamp;
    
    //Loop back what is read
    Serial.write(lineBuf[charCnt]);

    // read the incoming byte:
    lcd.write(lineBuf[charCnt]);
    
    charCnt++;
  }
}

