/*!
 * @file Basic.ino
 * 
 * @brief Basic example for DYP-R01CW laser ranging sensor
 * 
 * This sketch demonstrates how to use the DYP-R01CW library to read
 * distance measurements from the sensor using the I2C (Wire) interface.
 * 
 * @section hardware Hardware Requirements
 * 
 * - Arduino board (Uno, Mega, ESP32, etc.)
 * - DYP-R01CW / DFRobot SEN0590 laser ranging sensor
 * - I2C connection:
 *   - SDA to Arduino SDA pin
 *   - SCL to Arduino SCL pin
 *   - VCC to supply voltage (3.3...5.0V)
 *   - GND to GND
 * 
 * @section author Author
 * 
 * Written by Matthias Prinke
 * 
 * @section license License
 * 
 * MIT License
 */

#include <Wire.h>
#include <DYP_R01CW.h>

// Create sensor object with default I2C address (0xE8 in 8-bit format)
DYP_R01CW sensor;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Basic Example");
  Serial.println("==============================================");
  
  // Initialize the sensor
  if (!sensor.begin()) {
    Serial.println("ERROR: Could not find DYP-R01CW sensor!");
    Serial.println("Please check wiring and I2C address.");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("DYP-R01CW sensor initialized successfully!");
  
  // Read and display software version
  uint16_t version = sensor.readSoftwareVersion();
  if (version != 0) {
    Serial.print("Software Version: 0x");
    Serial.println(version, HEX);
  } else {
    Serial.println("WARNING: Could not read software version");
  }
  
  Serial.println();
  delay(1000);
}

void loop() {
  // Read distance from sensor
  int16_t distance = sensor.readDistance();
  
  // Check if reading was successful
  if (distance >= 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("ERROR: Failed to read distance");
  }
  
  // Wait before next reading
  delay(500);
}
