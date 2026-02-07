/*!
 * @file ChangeAddress.ino
 * 
 * @brief Example demonstrating how to change the I2C address of DYP-R01CW sensor
 * 
 * This sketch shows how to change the I2C address of the DYP-R01CW laser ranging
 * sensor. The sensor supports 20 different addresses from 0xD0 to 0xFE (even
 * addresses only). The default address is 0xE0.
 * 
 * IMPORTANT: After changing the address, you must create a new sensor object
 * with the new address to communicate with the sensor.
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

// Current I2C address (change this to match your sensor's current address)
#define CURRENT_ADDRESS 0x50

// New I2C address to set (must be one of: 0xD0, 0xD2, 0xD4, ..., 0xFE)
#define NEW_ADDRESS 0xE0

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Change Address Example");
  Serial.println("========================================================");
  Serial.println();
  
  // Create sensor object with current address
  DYP_R01CW sensor(CURRENT_ADDRESS);
  
  // Initialize the sensor
  Serial.print("Initializing sensor at address 0x");
  Serial.print(CURRENT_ADDRESS, HEX);
  Serial.println("...");
  
  if (!sensor.begin()) {
    Serial.println("ERROR: Could not find DYP-R01CW sensor!");
    Serial.println("Please check:");
    Serial.println("  1. Wiring connections");
    Serial.println("  2. Current I2C address matches CURRENT_ADDRESS");
    Serial.println("  3. Sensor is powered on");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("Sensor found!");
  Serial.println();
  
  // Read current software version
  uint16_t version = sensor.readSoftwareVersion();
  if (version != 0) {
    Serial.print("Software Version: 0x");
    Serial.println(version, HEX);
  }
  Serial.println();
  
  // Change the address
  Serial.print("Changing sensor address to 0x");
  Serial.print(NEW_ADDRESS, HEX);
  Serial.println("...");
  
  if (sensor.setAddress(NEW_ADDRESS)) {
    Serial.println("SUCCESS: Address changed successfully!");
    Serial.println();
    Serial.print("Sensor is now at address 0x");
    Serial.println(NEW_ADDRESS, HEX);
    Serial.println();
    
    // Verify by creating a new sensor object with the new address
    Serial.println("Verifying new address...");
    delay(100); // Short delay to ensure address change is complete
    
    DYP_R01CW newSensor(NEW_ADDRESS);
    if (newSensor.begin()) {
      Serial.println("Verification successful! Sensor responds at new address.");
      Serial.println();
      
      // Test reading distance with new address
      Serial.println("Testing distance measurement with new address:");
      int16_t distance = newSensor.readDistance();
      if (distance >= 0) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
      } else {
        Serial.println("Could not read distance (this is normal if nothing is in range)");
      }
    } else {
      Serial.println("WARNING: Could not verify new address");
      Serial.println("The address may have been changed, but verification failed.");
    }
  } else {
    Serial.println("ERROR: Failed to change address!");
    Serial.println("Possible reasons:");
    Serial.println("  1. Invalid address (must be 0xD0, 0xD2, 0xD4, ..., 0xFE)");
    Serial.println("  2. Communication error with sensor");
    Serial.println("  3. Sensor does not support address change");
  }
  
  Serial.println();
  Serial.println("Example complete.");
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}
