
// mp3 player
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// rangefinder
#include <math.h>
#include <SPI.h>

// rangefinder
#define echoPin 8 // Echo Pin
#define trigPin 9 // Trigger Pin
#define LEDPin 13 // Onboard LED

int maximumRange = 450; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
float feetDistance; // stores distance in feet
float inchDistance;

// mp3 player
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
 // setup rangefinder
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
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
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 Serial.println(distance);
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 
 // Uncomment this line to know how often we are getting out range data... 
 // Sensor supposed to be good to 14-15 feet.
   Serial.println("-1");
   digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 
   Serial.println(distance);
 // change to feet and round. that's all we need...
 feetDistance = distance * 0.0328084;
 inchDistance = feetDistance * 12;
 
 Serial.print("Feet: "); 
 Serial.print(round(feetDistance));
 Serial.print(" - Inches: ");
 Serial.println(round(inchDistance));
 
 digitalWrite(LEDPin, LOW); 


  // play mp3 if range is < 2 feet
  if(inchDistance < 6) {
    Serial.println("playing music 1");
    //myDFPlayer.play(1);  //Play the first mp3
    myDFPlayer.playMp3Folder(1);
    delay(8000);
  } else if(inchDistance < 20) {
    Serial.println("playing music 2");
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


