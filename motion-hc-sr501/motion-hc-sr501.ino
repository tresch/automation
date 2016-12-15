//Henry's Bench
// HC-SR501 Motion Detector
// Sample Sketch
// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-hc-sr501-motion-sensor-tutorial/
// Good explanation of use of jumper on sensor on the page.

int ledPin = 12;  // LED on Pin 13 of Arduino
int ledPin2 = 11;
int pirPin1 = 7; // Input for HC-S501
int pirPin2 = 6;

int pirValue1; // Place to store read PIR Value
int pirValue2;

void setup() {
  
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
 
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  
}

void loop() {
  pirValue1 = digitalRead(pirPin1);
  pirValue2 = digitalRead(pirPin2);
  
  //Serial.println(pirValue);
  
  if (pirValue1 == 1 or pirValue2 == 1) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
    //Serial.println("in ON");
  }
  else {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    //Serial.println("in off");
  }
  
  //digitalWrite(ledPin, pirValue);
  //digitalWrite(ledPin2, pirValue);

  //delay(100);
}
