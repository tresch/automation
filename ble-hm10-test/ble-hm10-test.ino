#include <SoftwareSerial.h> 
 
#define RxD 2
#define TxD 3
SoftwareSerial mySerial(RxD,TxD); 
 
void setup() 
{ 
    pinMode(RxD, INPUT); 
    pinMode(TxD, OUTPUT); 
    mySerial.begin(9600);               // the ble4.0 baud rate  
    Serial.begin(9600);                 // the terminal baud rate   
} 
 
void loop() 
{ 
    if(Serial.available()) 
    { 
       mySerial.print((char)Serial.read()); 
    }  
 
    if(mySerial.available()) 
    { 
       Serial.print((char)mySerial.read()); 
    }
}
