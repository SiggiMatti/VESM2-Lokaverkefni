// Include Arduino Wire library for I2C
#include <Wire.h>
#include <Servo.h> 
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
#define echoPin 4 
#define trigPin 2


void receiveEvent(int); // <--- Bæta þessari línu við.
void requestEvent();

int enablePin = 3;
int in1Pin = 5;
int in2Pin = 6;

// Declare the Servo pin 
int servoPin1 = 7; 
int servoPin2 = 8;

// Create a servo object 
Servo Servo1;
Servo Servo2;

// Variable for received data
int rd;

boolean reverse = 0;
 
void setup() {
  
  // Tengi servo-ana við pinnana sem þeir eru í
  Servo1.attach(servoPin1); 
  Servo2.attach(servoPin2);
 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  // Initialize I2C communications as Slave
  Wire.begin(8);

  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent); 
  
  
}

void loop() {
  if (rd == 4) {
    setMotor(200, reverse);
  }
  else if (rd == 3) {
    setMotor(200, ! reverse);
  }
  else if (rd == 2) {
    Servo1.write(45); 
    Servo2.write(45); 
  }
  else if (rd == 1) {
    Servo1.write(135); 
    Servo2.write(135); 
  } else {
    Servo1.write(90);
    Servo2.write(90); 
    setMotor(0, reverse);
  }
}

void setMotor(int speed, boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
}

void receiveEvent(int howMany) {
  // read one character from the I2C
  rd = Wire.read();
  // Print value of incoming data
  Serial.println(rd);
    
}

void requestEvent()                                //This Function is called when Master wants value from slave
{
  long duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  byte SlaveSend = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor    // Convert potvalue digital value (0 to 1023) to (0 to 127)
  // sends one byte converted POT value to master
  Wire.write(SlaveSend);
}
