// Sæki Wire library til að geta sent upplýsingar milli arduinoana og sæki liquid crystal library fyrir lcd display.
#include <Wire.h>
#include <LiquidCrystal.h>
 
// Defina slave addressið
#define SLAVE_ADDR 9

// Set upp pinnanna sem að eru tengdir við lcd displayið
LiquidCrystal lcd(5, 6, 8, 9, 10, 11);
// Geri variables fyrir takkanna
int buttonVinstri = 1;
int buttonHaegri = 2;
int buttonAftur = 3;
int buttonAfram = 4;
// Geri variables fyrir Stöðu takkanna
int buttonState = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
 
void setup() {
  // SEt upp lcd-ið
  lcd.begin(16, 2);
  Serial.begin(9600);
  
  // Geri alla takkanna á fjarstýringunni sem input pullup til að nota resistora í arduinoinum til að minnka tengingar á takkanum verulega.
  pinMode(buttonVinstri, INPUT_PULLUP);
  pinMode(buttonHaegri, INPUT_PULLUP);
  pinMode(buttonAftur, INPUT_PULLUP);
  pinMode(buttonAfram, INPUT_PULLUP);
  // Byrja I2C sem master.
  Wire.begin();
}
  
 
void loop() {
  
  Wire.requestFrom(8,1); // Spyt um byte frá slave arduino-inum.
  // Geymi það sem að ég fæ frá lave arduino-inum í MasterReceive og prenta það út á lcd-ið með lcd.print
  byte MasterReceive = Wire.read();
  lcd.print(MasterReceive);

  // Les frá öllum tökkunum á fjarstýringunni.
  buttonState = digitalRead(buttonVinstri);
  buttonState2 = digitalRead(buttonHaegri);
  buttonState3 = digitalRead(buttonAftur);
  buttonState4 = digitalRead(buttonAfram);

  // Kíki hvaða takka notandi er að ýta á og sendi númer sem að fer eftir áttinni sem að notandi ýtti á.
  Wire.beginTransmission(8);
  if (buttonState == 0) {
    byte state = 1;
    Wire.write(state);
    Wire.endTransmission();
  } else if (buttonState2 == 0) {
    Wire.write(2);
    Wire.endTransmission();
  } else if (buttonState3 == 0) {
    Wire.write(3);
    Wire.endTransmission();
  } else if (buttonState4 == 0) {
    Wire.write(4);
    Wire.endTransmission();
  } else {
    Wire.write(0);
    Wire.endTransmission();
  }
  // Geri smá delay svo að upplýsingarnar sjást á lcd-inu.
  delay(50);
  lcd.clear();
  
}
