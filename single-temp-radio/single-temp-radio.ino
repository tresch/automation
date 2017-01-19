#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

/*-----( Declare Constants and Pin Numbers )-----*/
// Data wire is plugged into port 2 on the Arduino (can be changed)
#define ONE_WIRE_BUS 2    // NOTE: No ";" on #define  


/*-----( Declare objects )-----*/
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


/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://arduino-info.wikispaces.com/Brick-Temperature-DS18B20#Read%20individual


 // WP 1
DeviceAddress Probe01 = { 0x28, 0x37, 0x1E, 0x66, 0x07, 0x00, 0x00, 0x4B }; // single probe
//DeviceAddress Probe02 = { 0x28, 0xEA, 0x58, 0x66, 0x07, 0x00, 0x00, 0xB0 }; // "4" // Air probe
//DeviceAddress Probe02 = { 0x28, 0x55, 0x0A, 0x66, 0x07, 0x00, 0x00, 0x85 }; // "5" 

// pool sensor 0x28, 0xFF, 0xB6, 0xDD, 0x63, 0x15, 0x01, 0x0B
//DeviceAddress Probe03 = { 0x28, 0x9A, 0x80, 0x40, 0x04, 0x00, 0x00, 0xD5 }; // "4" Again for test
//DeviceAddress Probe04 = { 0x28, 0x10, 0xA4, 0x57, 0x04, 0x00, 0x00, 0xA9 };


void setup() 
{
  Serial.begin(57600); //Debug 
//------- Initialize the Temperature measurement library--------------
  sensors.begin();
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 10);
  //sensors.setResolution(Probe02, 10);
  //sensors.setResolution(Probe03, 10);
  //sensors.setResolution(Probe04, 10);

  //nRF24 configurations
  radio.begin();
  radio.setChannel(0x4c);
  radio.setAutoAck(1);
  radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(32);
  radio.openReadingPipe(1,pipes[0]);
  radio.openWritingPipe(pipes[1]);
  radio.startListening();
  radio.printDetails(); //for Debugging

}//--(end setup )---


void loop() {
  
  sensors.requestTemperatures(); // Send the command to get temperatures

  float temp1 = displayTemperature(Probe01);  
  //float temp2 = displayTemperature(Probe02);  
  float temp2 = 0/0;
  
  char payload[31];
  
  String payloadString = buildPayloadString(temp1, temp2);
  payloadString.toCharArray(payload, 31);
 
  //displayTemperature(Probe03);  
  //displayTemperature(Probe04);  
  
  Serial.println(payload);
  
  // send the payload over the radio
  radio.stopListening();
  bool ok = radio.write(&payload,strlen(payload));
  radio.startListening(); 
  
  Serial.println("sent payload"); 
  
  delay(20000);
}

String buildPayloadString(float t1, float t2) {
  
  String payloadString = "";
  
  payloadString.concat("TVROOM,"); // Node indicator
  payloadString.concat("1,"); // number of data elements to expect
  payloadString.concat(t1);
  payloadString.concat(",");
  payloadString.concat(t1);
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
