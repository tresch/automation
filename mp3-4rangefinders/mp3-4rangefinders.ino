
// mp3 player
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// rangefinder
#include <math.h>
#include <SPI.h>

// rangefinder
#define echoPin1 2 // Echo Pin
#define trigPin1 3 // Trigger Pin
#define echoPin2 4 // Echo Pin
#define trigPin2 5 // Trigger Pin
#define echoPin3 6 // Echo Pin
#define trigPin3 7 // Trigger Pin
#define echoPin4 8 // Echo Pin
#define trigPin4 9 // Trigger Pin

#define LEDPin 13 // Onboard LED

int maximumRange = 450; // Maximum range needed
int minimumRange = 0; // Minimum range needed

long duration1, distance1; // Duration used to calculate distance
long duration2, distance2;
long duration3, distance3;
long duration4, distance4;

float feetDistance1, inchDistance1;
float feetDistance2, inchDistance2;
float feetDistance3, inchDistance3;
float feetDistance4, inchDistance4;

// mp3 player
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
 // setup rangefinders
 pinMode(trigPin1, OUTPUT);
 pinMode(echoPin1, INPUT);

 pinMode(trigPin2, OUTPUT);
 pinMode(echoPin2, INPUT);

 pinMode(trigPin3, OUTPUT);
 pinMode(echoPin3, INPUT);

 pinMode(trigPin4, OUTPUT);
 pinMode(echoPin4, INPUT);
 
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

  
  // setup mp3 player
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  //myDFPlayer.play(1);  //Play the first mp3
}

void loop()
{
  /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 

 // rangefinder 1
 digitalWrite(trigPin1, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance1 = duration1/58.2;

 digitalWrite(trigPin2, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance2 = duration2/58.2;

 digitalWrite(trigPin3, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin3, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin3, LOW);
 duration3 = pulseIn(echoPin3, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance3 = duration3/58.2;

 digitalWrite(trigPin4, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin4, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin4, LOW);
 duration4 = pulseIn(echoPin4, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance4 = duration4/58.2;

 
 Serial.print("d1: ");
 Serial.print(distance1);
 Serial.print(" d2: ");
 Serial.print(distance2);
 Serial.print(" d3: ");
 Serial.print(distance3);
 Serial.print(" d4: ");
 Serial.println(distance4);
 
 
 if (distance1 >= maximumRange || distance1 <= minimumRange || distance2 >= maximumRange || distance2 <= minimumRange
     || distance3 >= maximumRange || distance3 <= minimumRange || distance4 >= maximumRange || distance4 <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 
 // Uncomment this line to know how often we are getting out range data... 
 // Sensor supposed to be good to 14-15 feet.
   Serial.println("-1");
   //digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 
 //Serial.println(distance);
 // change to feet and round. that's all we need...
 feetDistance1 = distance1 * 0.0328084;
 inchDistance1 = feetDistance1 * 12;

 feetDistance2 = distance2 * 0.0328084;
 inchDistance2 = feetDistance2 * 12;

 feetDistance3 = distance3 * 0.0328084;
 inchDistance3 = feetDistance3 * 12;

 feetDistance4 = distance4 * 0.0328084;
 inchDistance4 = feetDistance4 * 12;

 Serial.println("Inches: ");
 Serial.print("d1: ");
 Serial.print(inchDistance1);
 Serial.print(" d2: ");
 Serial.print(inchDistance2);
 Serial.print(" d3: ");
 Serial.print(inchDistance3);
 Serial.print(" d4: ");
 Serial.println(inchDistance4);

 /*
 Serial.print("Feet: "); 
 Serial.print(round(feetDistance));
 Serial.print(" - Inches: ");
 Serial.println(round(inchDistance));
 
 digitalWrite(LEDPin, LOW); 
*/

  // play mp3 if range is < 2 feet
  if(inchDistance1 < 3 || inchDistance2 < 3 || inchDistance3 < 3 || inchDistance4 < 3) {
    Serial.println(" playing music 1");
    //myDFPlayer.play(1);  //Play the first mp3
    myDFPlayer.playMp3Folder(1);
    delay(8000);
  } else if(inchDistance1 < 8 || inchDistance2 < 8 || inchDistance3 < 8 || inchDistance4 < 8) {
    Serial.println(" playing music 2");
    myDFPlayer.playMp3Folder(2);
    delay(8000);
  } else {
    Serial.println("in else");
  }

 
 }
 
 //Delay 50ms before next reading.
 delay(500);

  /*
  static unsigned long timer = millis();
  
  if (millis() - timer > 8000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  */
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}


