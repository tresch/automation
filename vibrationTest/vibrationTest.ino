int led = 13 ;// define LED Interface
int sensor1 = 3; // define the vibration sensor interface
int sensor2 = 2;
int val1; // define numeric variables val
int val2;

void setup ()
{
  Serial.begin(9600);
  
  pinMode (led, OUTPUT) ; // define LED as output interface
  pinMode (sensor1, INPUT) ; // output interface defines vibration sensor
  pinMode(sensor2,INPUT);
}
void loop ()
{
   val1 = digitalRead(sensor1) ; // read digital interface is assigned a value of 3 val
   val2 = digitalRead(sensor2) ;

//Serial.println(val);
  
  if (val1 == HIGH) // When the shock sensor detects a signal, LED flashes
  {
    digitalWrite (led, HIGH);
    Serial.println("trigger 1");
  }
  else
  {
    digitalWrite (led, LOW);
    //Serial.println("low");
  }

  if (val2 == HIGH) // When the shock sensor detects a signal, LED flashes
  {
    digitalWrite (led, HIGH);
    Serial.println("trigger 2");
  }
  else
  {
    digitalWrite (led, LOW);
    //Serial.println("low");
  }
}
