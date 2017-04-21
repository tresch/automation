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
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
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

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int buttonPin = 8;
int buttonState = 0;
int pressState = 1; // on off
int lastState = 0;

String initText = "Hello Julia!";
String julia = "Julia";
int stateChanged = 0;
int randIdx;

String nameList[62] = {"Greg","Amaya","Connor","Gecko","Catboy","Owlette","Reese","Dot","Dee","Del","Sid","Gabriella","Gerald",
                        "May","Emma","Chloe","Enzo","Snithik","Sydney","Zoe","Addie G","Adelle","David","Easton","Lily","Addy O",
                        "Jackson","Celeste","Parker","Jack","Tyler","Darius","Addison","Jacob","Beantley","Domanic","Olivia","Avery",
                        "Lexi","Kassie","Kinsley","Kylee","Hailey","Alivia","Prteekshya","Shelby","Austin","Allie","Xander",
                        "Ian","Adam","Mason","Victoria","Amanda","Dotty","Casey","Candy","Romeo","Night Ninja","Ninjalenos","Luna Girl",
                        "Moths"};

void setup() {

  randomSeed(analogRead(0));

  // set up button pin
  pinMode(buttonPin, INPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello Julia!");
  delay(3000);
  lcd.setCursor(0, 1);
  lcd.print("Ready to change?");
}

void loop() {
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);

  buttonState = digitalRead(buttonPin);
  /*
  if (buttonState == HIGH){  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pressed 1st row");
    lcd.setCursor(0, 1);     // Here I changed so the text is displayed on the next row.
    lcd.print("2nd row");
  }
  else {                             
    lcd.setCursor(0, 1);
    lcd.print("Not pressed"); 
  }
  */

  // flip the pressState on a button press
  if(buttonState == HIGH && lastState == 0) {
    if(pressState == 0) {
      pressState = 1;
    } else {
      pressState = 0;
    }
    stateChanged = 1;
    lastState = 1;
  } else {
    stateChanged = 0;
    lastState = 0;
  }

  if(stateChanged == 1) {
    if(pressState ==  1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Julia becomes...");
      lcd.setCursor(0, 1);     // Here I changed so the text is displayed on the next row.

      randIdx = (int)random(62);

      String currName = nameList[randIdx];
      
      lcd.print(currName);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You turn back to:");
      lcd.setCursor(0, 1);     // Here I changed so the text is displayed on the next row.
      lcd.print("Julia!!!");
    }
  }

  delay(100);
  
}

