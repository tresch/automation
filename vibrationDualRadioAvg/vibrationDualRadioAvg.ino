#include "nRF24L01.h"
#include "RF24.h"

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


int led = 13 ;// define LED Interface
int sensor1 = 2; // washer
int sensor2 = 3; // dryer
float val1; // define numeric variables val
float val2;

int washerStatus = 0;
int dryerStatus = 0;

void setup ()
{
  Serial.begin(9600);
  
  pinMode (led, OUTPUT) ; // define LED as output interface
  pinMode (sensor1, INPUT) ; // output interface defines vibration sensor
  pinMode(sensor2,INPUT);

  // nRF24 configurations
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
}
void loop ()
{
   val1 = 0.0;
   val2 = 0.0;

  int numLoops = 100;

   for(int i=0;i<numLoops;i++) {
    val1 += digitalRead(sensor1);
    val2 += digitalRead(sensor2);
    delay(50);
   }

   Serial.print("val1: ");
   Serial.print(val1);
   Serial.print("  val2: ");
   Serial.println(val2);

   if( (val1 / numLoops) > 0.0 ) {
      washerStatus = 1;
   } else {
      washerStatus = 0;
   }

   if( (val2 / numLoops) > 0.0 ) {
      dryerStatus = 1;
   } else {
      dryerStatus = 0;
   }


  if(dryerStatus == 1 || washerStatus == 1) {
    char payload[31];
  
    String payloadString = buildPayloadString(washerStatus, dryerStatus);
    payloadString.toCharArray(payload, 31);
  
    Serial.println(payload);
  
    // send the payload over the radio
    radio.stopListening();
    bool ok = radio.write(&payload,strlen(payload));
    radio.startListening(); 
  
    Serial.println("sent payload"); 
  }
  
  delay(5000);
}

String buildPayloadString(float t1, float t2) {
  
  String payloadString = "";
  
  payloadString.concat("LAUNDRY,"); // Node indicator
  payloadString.concat("2,"); // number of data elements to expect
  payloadString.concat(t1);
  payloadString.concat(",");
  payloadString.concat(t2);
  payloadString.concat(",");
  
  return payloadString;
}
