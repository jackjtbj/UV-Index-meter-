#include "ML8511.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);  // Address, columns, rows

const int analogPin = A0;   // The AN pin from the UV Click
const int enablePin = 7;    // The RST pin from the UV Click

ML8511 uvSensor(analogPin, enablePin);

void setup() {
  Serial.begin(9600);
  Serial.println("UV Sensor Test");
  
  lcd.init();          
  lcd.backlight();
}

// Reads UV intensity and returns the category
String showUVCategory(float maxUV, byte &categ) { 
  char strCat[12];
  categ = (byte)(maxUV + 0.5);  

  if (categ < 3) {
    strcpy(strCat, "LOW");
  } else if (categ < 6) {
    strcpy(strCat, "MODERATE");
  } else if (categ < 8) {
    strcpy(strCat, "HIGH");
  } else if (categ < 11) {
    strcpy(strCat, "VERY HIGH");
  } else {
    strcpy(strCat, "EXTREME");
  }
  return String(strCat);
}

void loop() {

  float uvIntensity = uvSensor.getUV();

  byte numCateg = 0;
  

  String uvCategory = showUVCategory(uvIntensity, numCateg);
  
  lcd.clear(); 
  
  // Line 1
  lcd.setCursor(0, 0);
  lcd.print("UV:");
  lcd.print(uvIntensity, 2);
  lcd.print(" mW/cm2");
  
  // Line 2
  lcd.setCursor(0, 1);
  lcd.print("Cat:");
  lcd.print(uvCategory);
  lcd.print(" (");
  lcd.print(uvSensor.estimateDUVindex(uvIntensity));
  lcd.print(")");
  Serial.print("Analog");
  
  // serial port
  Serial.print(analogPin);
  Serial.print("UV Intensity: ");
  Serial.print(uvIntensity);
  Serial.print(" mW/cm2 ");
  Serial.print("Category: ");
  Serial.print(uvCategory);
  Serial.print(" (");
  Serial.print(uvSensor.estimateDUVindex(uvIntensity));
  Serial.println(")");
  delay(1000);  // Wait 1 seconds
}
