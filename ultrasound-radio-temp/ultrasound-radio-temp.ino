/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED

#define ONE_WIRE_BUS 2    // NOTE: No ";" on #define -- radio

/* temp */
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass address of our oneWire instance to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/* radio */
//for nrf24 debug
int serial_putc( char c, FILE * ) 
{
  Serial.write( c );
  return c;
} 

//for nrf24 debug
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

//nRF24 set the pin 9 to CE and 10 to CSN/SS
// Cables are:
//     SS       -> 10
//     MOSI     -> 11
//     MISO     -> 12
//     SCK      -> 13

RF24 radio(9,10);

//we only need a write pipe, but am planning to use it later
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL,0xF0F0F0F0D2LL };

/* end radio */

/* temp probe */
DeviceAddress Probe01 = { 0x28, 0xC6, 0x0A, 0x67, 0x07, 0x00, 0x00, 0xE2 }; // "4"

int maximumRange = 450; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
float feetDistance; // stores distance in feet
float inchDistance;

void setup() {
  
 Serial.begin (9600);
 
 // Ultrasonic
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
 
 // temp
 //------- Initialize the Temperature measurement library--------------
  sensors.begin();
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 10);
 
 // radio
 //nRF24 configurations
  radio.begin();
  radio.setChannel(0x4c);
  radio.setAutoAck(1);
  radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(32);
  radio.openReadingPipe(1,pipes[0]);
  radio.openWritingPipe(pipes[1]);
  //radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  radio.printDetails(); //for Debugging
}

void loop() {
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
 
 //Serial.println(distance);
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 
 // Uncomment this line to know how often we are getting out range data... 
 // Sensor supposed to be good to 14-15 feet.
 //Serial.println("-1");
 digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 
 //Serial.println(distance);
 // change to feet and round. that's all we need...
 feetDistance = distance * 0.0328084;
 inchDistance = feetDistance * 12;
 
 Serial.print("Feet: "); 
 Serial.print(round(feetDistance));
 Serial.print(" - Inches: ");
 Serial.println(round(inchDistance));
 
 digitalWrite(LEDPin, LOW); 
 }
 
 // get Temperature
 sensors.requestTemperatures(); // Send the command to get temperatures
 float temp1 = displayTemperature(Probe01);
 
 // generate and send the payload
 char payload[31];
  
 String payloadString = buildPayloadString(feetDistance, temp1);
 payloadString.toCharArray(payload, 31);
 
 Serial.println(payload);
  
  // send the payload over the radio
  radio.stopListening();
  bool ok = radio.write(&payload,strlen(payload));
  radio.startListening(); 
  
  Serial.println("sent payload"); 
 
 //Delay 50ms before next reading.
 delay(17000);
}

String buildPayloadString(float data1, float data2) {
  
  String payloadString = "";
  
  payloadString.concat("GARAGE,"); // Node indicator
  payloadString.concat("1,"); // number of data elements to expect
  payloadString.concat(data1);
  payloadString.concat(",");
  payloadString.concat(data2);
  payloadString.concat(",");
  
  return payloadString;
}

float displayTemperature(DeviceAddress deviceAddress) {


  float tempC = sensors.getTempC(deviceAddress);
  float tempF = DallasTemperature::toFahrenheit(tempC);

   if (tempC == -127.00) // Measurement failed or no device found
   {
    Serial.println("Temperature Error");
   }
  /* 
   else
   {
   //lcd.print("C=");
   //lcd.print(tempC);
   Serial.print(" F=");
   Serial.println(tempF); // Convert to F
   }
   */
   return tempF;
}
