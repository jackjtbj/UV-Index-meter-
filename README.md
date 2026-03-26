UV Intensity Monitor

An Arduin UV monitoring system that displays real-time UV measurements on an LCD screen.

Features

- Real-time UV intensity display (mW/cm²)
- UV Index estimation
- UV category (LOW to EXTREME)
- Updates every second
- plays a warning sound after reaching a unsafe UV index level of 5

Hardware Required

- Arduino UNO board
- ML8511 UV Sensor
- 16x2 I2C LCD display
- a Df robot speaker 2

Software Required

- Arduino IDE
- Libraries: Wire.h, LiquidCrystal_I2C.h, ML8511.h

Quick Start

1. Install required libraries via Arduino Library Manager
2. Connect hardware according to your setup
3. Upload the code to your Arduino
4. Open Serial Monitor (9600 baud) for additional data

The LCD shows:
- Line 1 UV intensity (mW/cm²)
- Line 2 UV level and index

Common Errors
- If LCD is engaging but not displaying correctly check for correct LCD adress ussualy a (0x20) like sequence use the address finder code to determine the adress and input it into
- LiquidCrystal_I2C lcd(**0x20**, 16, 2);
  
- 
  
