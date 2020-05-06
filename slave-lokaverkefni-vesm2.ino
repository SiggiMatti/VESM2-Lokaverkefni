// Sæki Wire library-ið svo að ég geti sent upplýsingar milli báða arduino-ana og ég sæki servo svo að ég geti stjórnað servo-unum
#include <Wire.h>
#include <Servo.h> 
 
// Defina slave I2C addressið og echoPin og trigPin fyrir ultrasonic sensorinn
#define SLAVE_ADDR 9
#define echoPin 4 
#define trigPin 2

// Geri receiveEvent og request sem að keyra smá kóða til að taka á móti og suprja eftir upplýsingum frá slave arduino
void receiveEvent(int); 
void requestEvent();

// Set upp pinnanna fyrir L293D chip-ið sem að stjórnar DC mótorunum
int enablePin = 3;
int in1Pin = 5;
int in2Pin = 6;

// Set upp báða servo pinnanna
int servoPin1 = 7; 
int servoPin2 = 8;

// Bæy til servo object fyrir báða servo-ana
Servo Servo1;
Servo Servo2;

// Breyta fyrir upplýsingar sem ég fæ frá master
int rd;

boolean reverse = 0;
 
void setup() {
  
  // Tengi servo-ana við pinnana sem þeir eru í
  Servo1.attach(servoPin1); 
  Servo2.attach(servoPin2);
 
  pinMode(trigPin, OUTPUT); // Læt trigPinnann sem OUTPUT
  pinMode(echoPin, INPUT); // Læt ehcoPinnann sem INPUT
  Serial.begin(9600); // // Set upp Serial
  
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  // Byrja I2C samskipti sem slave
  Wire.begin(8);
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent); 
  
  
}

void loop() {
  // Ég sendi númer frá masterinum til slave eftir því hvaða átt notandi ýtir á tökkunum á fjarstýringunni og eftir því hvaða átt hann velur þá læt ég bílinn fara í rétta átt
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

// Hérna er tekið inn tvær breytur speed sem að velur hversu hratt DC mótorinn fer og tekur inn breytu reverse sem að er venjulega 0 en ef notandi velur að bakka þá verður breytan 1 og bíllinn bakkar eða fer áfram eftir því
void setMotor(int speed, boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
}

void receiveEvent(int howMany) {
  // Tek inn upplýsingar frá master-inum
  rd = Wire.read();
  // Prenta það sem mér er sent
  Serial.println(rd);
    
}

// Þegar að masterinn vill fá upplýsingar frá slave-inum þá keyrir þessi kóði
void requestEvent()
{
  // Hérna keyrir smá kóði sem að finnur út hversu marga sentimetra hlutur er í burtu frá ultrasonic sensorinum
  long duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Lætur trigPinnann HIGH í tíu míkrósekúndur
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Lesur echoPinnann og skilar hversu lengi hljóðið tók að fara til hlutsins
  duration = pulseIn(echoPin, HIGH);
  // Formúla til að finna út hvernig maður breytir tímanum í sentimetra.
  byte SlaveSend = duration * 0.034 / 2;
  // Ég sendi síðan hversu marga sentimetra hlutur er frá ultrasonic sensorinum til mastersins svo hann getur sýnt það á lcd display-inu
  Wire.write(SlaveSend);
}
