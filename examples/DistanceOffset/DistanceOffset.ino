/*!
 * @file DistanceOffset.ino
 * 
 * @brief Example demonstrating distance offset functionality for DYP-R01CW sensor
 * 
 * This sketch demonstrates how to set and use a distance offset with the
 * DYP-R01CW library. The offset can be used to calibrate the sensor or
 * compensate for mounting distance.
 * 
 * @section hardware Hardware Requirements
 * 
 * - Arduino board (Uno, Mega, ESP32, etc.)
 * - DYP-R01CW / DFRobot SEN0590 laser ranging sensor
 * - I2C connection:
 *   - SDA to Arduino SDA pin
 *   - SCL to Arduino SCL pin
 *   - VCC to 5V (or 3.3V depending on sensor version)
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
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Distance Offset Example");
  Serial.println("=========================================================");
  
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
  // Demonstrate reading with default offset (0 mm)
  Serial.println("--- Reading with default offset (0 mm) ---");
  int16_t distance = sensor.readDistance();
  if (distance >= 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("ERROR: Failed to read distance");
  }
  
  delay(1000);
  
  // Demonstrate reading with positive offset (+10 mm)
  Serial.println("\n--- Setting offset to +10 mm ---");
  sensor.setDistanceOffset(10);
  Serial.print("Current offset: ");
  Serial.print(sensor.getDistanceOffset());
  Serial.println(" mm");
  
  distance = sensor.readDistance();
  if (distance >= 0) {
    Serial.print("Distance (with +10mm offset): ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("ERROR: Failed to read distance");
  }
  
  delay(1000);
  
  // Demonstrate reading with negative offset (-5 mm)
  Serial.println("\n--- Setting offset to -5 mm ---");
  sensor.setDistanceOffset(-5);
  Serial.print("Current offset: ");
  Serial.print(sensor.getDistanceOffset());
  Serial.println(" mm");
  
  distance = sensor.readDistance();
  if (distance >= 0) {
    Serial.print("Distance (with -5mm offset): ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("ERROR: Failed to read distance");
  }
  
  delay(1000);
  
  // Reset to no offset
  Serial.println("\n--- Resetting offset to 0 mm ---");
  sensor.setDistanceOffset(0);
  Serial.print("Current offset: ");
  Serial.print(sensor.getDistanceOffset());
  Serial.println(" mm\n");
  
  delay(2000);
}
