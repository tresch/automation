#include <SoftwareSerial.h>
#include <math.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// for hm-10 module
  int led         = 13;
  int bluetoothTx = 2;
  int bluetoothRx = 3;
  SoftwareSerial bluetooth(bluetoothTx, bluetoothRx); 

  
// end hm-10 module  
  
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
  
  // program specific variables
  int i = 1;
  int counter = 0;
  double avgDistance = 0.0;
  
  // use a queue for average of last N later
  double d1 = 0.0;
  double d2 = 0.0;
  double d3 = 0.0;
  double d4 = 0.0;
  double d5 = 0.0;
  

void setup() {
  Serial.begin(9600);
  
  // set up radio
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
  
  // set up bluetooth
  bluetooth.begin(9600);
  while(!Serial){}

  bluetooth.write("AT");
  while (bluetooth.available()) {
     Serial.write(bluetooth.read());
   }
  Serial.write("AT sent");
  delay(500);
  bluetooth.write("AT+NAME?");
  delay(500);
  while (bluetooth.available()) {
     Serial.write(bluetooth.read());
   }
  delay(100);
  Serial.println("");

  //pinMode(led,OUTPUT);
  //digitalWrite(led,HIGH);
  
  bluetooth.write("AT+ROLE1");
  delay(100);
  while (bluetooth.available()) {
     Serial.write(bluetooth.read());
   }
  delay(100);
  Serial.println("");
  
  delay(500);
  bluetooth.write("AT+IMME1");
  delay(500);
  while (bluetooth.available()) {
     Serial.write(bluetooth.read());
   }
  delay(100);
  Serial.println("");
}

void loop()
{
  bluetooth.write("AT+DISI?");
  //delay(500);
  
  String tags = "";
  
  //while (bluetooth.available() > 0) {
  while(!tags.endsWith("OK+DISCE")) {
     char character;
     //Serial.write(bluetooth.read());
     if(bluetooth.available() > 0) {
       character = bluetooth.read();
       //Serial.write(character);
       tags = tags + character;
     }
   }
   //Serial.println("");
   //delay(50);
  //Serial.println("done reading. tags is: ");
  
  //Serial.println(tags);
  
  int uuidIndex = tags.indexOf("8D773D3823A04DACAA99643641521901");
  
  //Serial.print("index is: ");
  //Serial.println(uuidIndex);
  
  if(uuidIndex > -1 && tags.length() >= (uuidIndex + 61)) {
    String message = tags.substring(uuidIndex, uuidIndex+61);
    //Serial.print("message:");
    //Serial.println(message);
    
    // consume message
    int lastColon = message.lastIndexOf(":");
    String rssi = message.substring(lastColon+1);
    //Serial.print("RSSI: ");
    //Serial.println(rssi);
    //Serial.print("RSSI int: ");
    //Serial.println(rssi.toInt());
    
    counter++;
    
    int txPower = -62; // c2 converted to binary then twos complement yields -62
    
    double distance = getDistance(rssi.toInt(), txPower);
    
    // need to average distance over last 5 readings or something...
    if(d1 == 0.0 && d2 == 0.0 && d3 == 0.0 && d4 == 0.0 && d5 == 0.0) {
      d1 = distance;
      d2 = distance;
      d3 = distance;
      d4 = distance;
      d5 = distance;
    } else {
      d1 = d2;
      d2 = d3;
      d3 = d4;
      d4 = d5;
      d5 = distance;
    }
    
    avgDistance = (d1 + d2 + d3 + d4 + d5) / 5.0;
    //Serial.print("raw dist: ");
    //Serial.print(distance);
    //Serial.print(" ");
    Serial.print("RSSI int: ");
    Serial.print(rssi.toInt());
    Serial.print(" - Feet: ");
    Serial.println(round(avgDistance));
  
    // send payload
    if(counter % 1 == 0) {
      // transmit distance
      // generate and send the payload
     char payload[31];
  
     String payloadString = buildPayloadString(round(avgDistance), getTemperatureInF());
     payloadString.toCharArray(payload, 31);
 
     Serial.print("payload: ");
     Serial.println(payload);
  
     // send the payload over the radio
     radio.stopListening();
     //Serial.print("stopped listening- payload length: ");
     //Serial.println(strlen(payload));
     bool ok = radio.write(&payload,strlen(payload));
     //Serial.println("wrote to radio");
     radio.startListening(); 
     //Serial.println("started listening");
  
     Serial.println("sent payload"); 
    }
  }
  
  //delay(3000);
}



double getDistance(int rssi, int txPower) {
    /*
     * RSSI = TxPower - 10 * n * lg(d)
     * n = 2 (in free space)
     * 
     * d = 10 ^ ((TxPower - RSSI) / (10 * n))
     */

    double meters = pow(10.0, ((double) txPower - rssi) / (10 * 2));
    //Serial.println(rssi);
    //Serial.println(txPower);
    //Serial.print("m: ");
    //Serial.println(meters);
    double feet = meters * 3.28084;

    return feet;
}

String buildPayloadString(float data1, float data2) {
  
  String payloadString = "";
  
  payloadString.concat("BLE2,"); // Node indicator
  payloadString.concat("1,"); // number of data elements to expect
  payloadString.concat(data1);
  payloadString.concat(",");
  payloadString.concat(data2);
  payloadString.concat(",");
  
  return payloadString;
}

float getTemperatureInF() {

  bluetooth.write("AT+TEMP?");
  Serial.println("AT+TEMP?");
  delay(50);
  
  String tempC = "";

  float tempF = 0.0;
  
  while (bluetooth.available() > 0) {
     char character;
       character = bluetooth.read();
       Serial.print(character);
       tempC = tempC + character;
   }

   int lastColon = tempC.lastIndexOf(":");
   tempC = tempC.substring(lastColon+1);

   if (tempC != "") {
      Serial.print("tempC: ");
      Serial.println(tempC);
      tempF = tempC.toFloat() * 1.8 + 32 - 9; // bs temp adjustment of 9 since reads too high.    
   }

   Serial.print("tempF: ");
   Serial.println(tempF);

   return tempF;
}


