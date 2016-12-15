#include <SoftwareSerial.h>

  int led         = 13;
  int bluetoothTx = 2;
  int bluetoothRx = 3;
  SoftwareSerial bluetooth(bluetoothTx, bluetoothRx); 
  int baudrate[8] ={4800,9600,14400,19200,28800,38400,57600,115200};
  int i = 1;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  while(!Serial){}

  Serial.write("AT sent");
  delay(500);
  bluetooth.write("AT+NAME?");
  delay(500);
  while (bluetooth.available()) {
     Serial.write(bluetooth.read());
   }
  delay(100);
  Serial.println("");

  bluetooth.write("AT+POWE3");
  delay(500);
  while(bluetooth.available()) 
  {
    Serial.write(bluetooth.read());
  }
  delay(100);
  Serial.println("");

  delay(500);
  bluetooth.write("AT+CHAR?");
  delay(500);
  while (bluetooth.available()) {
     Serial.write(bluetooth.read());
   }
  delay(100);
  Serial.println("");

  delay(500);
  bluetooth.write("AT+NAMEFlightline"); //Check Status
  delay(500);
  while (bluetooth.available()) {
      Serial.write((char)bluetooth.read());

    }

  Serial.println("");
  bluetooth.write("AT+CHAR0x2901"); //add charicteristic
  delay(500);
  while (bluetooth.available()) {
      Serial.write(bluetooth.read());

    }
  Serial.println("");
  bluetooth.write("AT+RELI0"); 
  delay(500);
  while (bluetooth.available()) {
      Serial.write(bluetooth.read());
    }
  Serial.println("");
  bluetooth.write("AT+SHOW1");
  delay(100);
  while (bluetooth.available()) {
      Serial.write(bluetooth.read());

    }
  Serial.println("");

  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
}

void testAllBaudRates(){
  for(int j=0; j<8; j++)
   {
      bluetooth.begin(baudrate[j]);
      delay(100);
      Serial.println("boud rate " + String(baudrate[j],DEC) +" i-> "+String(j,DEC));
     // Serial.println("");
      bluetooth.write("AT");
      delay(500);
      while (bluetooth.available()) {
        Serial.write(bluetooth.read());
        Serial.println();
       }
       delay(100);
   }
}                                            

// and now a few blinks of the  LED, 
// so that we know the program is running

void loop()
{
  //Read from bluetooth and write to usb serial
  while(bluetooth.available())
  {
    char toSend = (char)bluetooth.read();
    if(toSend == 'x'){
       digitalWrite(led,HIGH);
       Serial.println("set high");
       bluetooth.write("RXOK");
    }else if(toSend == 'y'){
      digitalWrite(led,LOW);
      Serial.println("set low");
      bluetooth.write("RXOK");
    }
    Serial.print(toSend);

  }

  //Read from usb serial to bluetooth
  while(Serial.available())
  {
    char toSend = (char)Serial.read();
    bluetooth.write(toSend);
    Serial.print(toSend);
  }
}
