// UV-detector
#include "ML8511.h" //uses ML8511 UV-detector libary
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //lcd display libary
#include <movingAvg.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);  // (Address, columns, rows) defines the boundarys of the lcd screen 

const int analogPin = A0;   // The AN pin is the Anaolog input pin from the UV detector
const int enablePin = 7;    // The RST pin for the UV-detector 

ML8511 uvSensor(analogPin, enablePin); //sets the 2 pins required for the uv sensor

// Create moving average objects
movingAvg uvFilter(10);      // For UV readings (n-point average)
movingAvg voltageFilter(10); // For voltage readings (n-point average)

void setup() {
  Serial.begin(9600); //starts serial output
  Serial.println("UV Sensor Test with Moving Average");

  //starts lcd back light and backlight
  lcd.init();          
  lcd.backlight();
  
  // Initialize the moving average filters
  uvFilter.begin();
  voltageFilter.begin();
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
  // Read raw UV intensity
  float rawUV = uvSensor.getUV();
  
  // Read raw voltage
  int analogValue = analogRead(analogPin);
  float rawVoltage = analogValue * (3.3 / 1023.0);
  
  //moving average to UV
  int rawUV_int = rawUV * 1000;  // Multiply by 1000 to keep 3 decimal places
  int filteredUV_int = uvFilter.reading(rawUV_int);
  float filteredUV = filteredUV_int / 1000.0;
  
  //moving average to voltage
  int rawVoltage_int = rawVoltage * 1000;
  int filteredVoltage_int = voltageFilter.reading(rawVoltage_int);
  float filteredVoltage = filteredVoltage_int / 1000.0;
  
  byte numCateg = 0;
  String uvCategory = showUVCategory(filteredUV, numCateg);
  
  lcd.clear(); 
  
  // Line 1
  lcd.setCursor(0, 0); // sets postion of cursor on the lcd screen to the top row
  lcd.print("UV:");
  lcd.print(filteredUV, 2); //prints the UV in mW/cm^2
  lcd.print(" mW/cm2");
  
  // Line 2
  lcd.setCursor(0, 1); // sets postion of cursor on the lcd screen to the bottom row
  lcd.print(uvCategory); finds
  lcd.print(" ");
  lcd.print("(");
  lcd.print(round(uvSensor.estimateDUVindex(filteredUV))); //finds the UV index 
  lcd.print(")");
  lcd.print(" ");
  lcd.print(filteredVoltage,2);
  lcd.print("V");

  
  // Serial output
  Serial.print("UV - Raw: ");
  Serial.print(rawUV, 3);
  Serial.print(" mW/cm2 | Filtered: ");
  Serial.print((filteredUV,2));
  Serial.println(" mW/cm2");
  
  Serial.print("Voltage - Raw: ");
  Serial.print(rawVoltage, 3);
  Serial.print(" V | Filtered: ");
  Serial.print(filteredVoltage, 3);
  Serial.println(" V");
  
  delay(1000);  // Wait 0.3 seconds between readings
}
