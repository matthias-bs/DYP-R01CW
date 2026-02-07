/*!
 * @file RestoreAddress.ino
 * 
 * @brief Example demonstrating how to restore the sensor address to default using broadcast
 * 
 * This sketch shows how to restore the I2C address of the DYP-R01CW laser ranging
 * sensor back to the default address (0xE8) using the I2C broadcast/general call address.
 * 
 * This is particularly useful when:
 * - You don't know what address the sensor is currently configured to
 * - You want to reset a sensor to factory default address
 * - You have changed the address and want to restore it
 * 
 * The broadcast address (0x00 in I2C) allows communication with all devices on the
 * I2C bus regardless of their configured address. By using the broadcast address,
 * we can restore any sensor to the default address without needing to know its
 * current address.
 * 
 * The sensor's default 8-bit address is 0xE8.
 * 
 * IMPORTANT: This uses the I2C broadcast address (0x00) to communicate with the
 * sensor regardless of its current address setting.
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

// I2C broadcast address (general call address)
// Note: 0x00 is the universal broadcast address in I2C (7-bit address format)
#define BROADCAST_ADDR 0x00

// Default address to restore to (8-bit format)
#define DEFAULT_ADDRESS_8BIT 0xE8

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Restore Address Example");
  Serial.println("=========================================================");
  Serial.println();
  
  Serial.println("This example will restore the sensor to its default address (0xE8)");
  Serial.println("using the I2C broadcast address, regardless of current address.");
  Serial.println();
  
  Serial.println("Attempting to restore sensor address to default (0xE8)...");
  Serial.println("Using broadcast address (0x00) - works with any current address.");
  Serial.println();
  
  // Create a sensor object using the broadcast address
  // The broadcast address allows communication with any sensor regardless of its current address
  DYP_R01CW broadcastSensor(BROADCAST_ADDR << 1); // Convert 7-bit to 8-bit format for constructor
  
  // Initialize the sensor object (sets up the Wire interface)
  // Note: begin() will likely return false since the broadcast address won't respond to 
  // read requests, but it initializes the Wire interface which is all we need for setAddress()
  bool beginResult = broadcastSensor.begin();
  Serial.print("INFO: Broadcast sensor begin() ");
  Serial.println(beginResult ? "succeeded (unexpected but OK)" : "failed (expected - broadcast address doesn't respond to reads)");
  Serial.println();
  
  // Use the DYP_R01CW setAddress method to restore the sensor to default address
  // This sends the address change command via the broadcast address
  // This works regardless of the begin() result above
  if (broadcastSensor.setAddress(DEFAULT_ADDRESS_8BIT)) {
    Serial.println("SUCCESS: Address restore command sent via broadcast address!");
    Serial.println();
  } else {
    Serial.println("WARNING: setAddress() returned false.");
    Serial.println("The sensor may still have been restored. Continuing verification...");
    Serial.println();
  }
  
  // Wait for sensor to process the address change
  delay(100);
  
  // Now try to communicate with the sensor at the default address
  Serial.println("Verifying sensor is now at default address (0xE8)...");
  
  DYP_R01CW sensor(DEFAULT_ADDRESS_8BIT);
  
  if (sensor.begin()) {
    Serial.println("SUCCESS: Sensor found at default address 0xE8!");
    Serial.println();
    
    // Read software version to confirm communication
    uint16_t version = sensor.readSoftwareVersion();
    if (version != 0) {
      Serial.print("Software Version: 0x");
      Serial.println(version, HEX);
    }
    Serial.println();
    
    // Test reading distance
    Serial.println("Testing distance measurement:");
    int16_t distance = sensor.readDistance();
    if (distance >= 0) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" mm");
    } else {
      Serial.println("Could not read distance (this is normal if nothing is in range)");
    }
    
    Serial.println();
    Serial.println("Address restoration complete!");
    Serial.println("The sensor is now configured with the default address 0xE8.");
  } else {
    Serial.println("ERROR: Could not find sensor at default address 0xE8!");
    Serial.println();
    Serial.println("Possible reasons:");
    Serial.println("  1. Sensor may not support broadcast address restoration");
    Serial.println("  2. Wiring connections may be incorrect");
    Serial.println("  3. Sensor may not be powered on (requires 3.3-5.0V)");
    Serial.println("  4. I2C pull-up resistors may be required (3-10kΩ)");
    Serial.println();
    Serial.println("If the sensor was at a non-default address, you may need to:");
    Serial.println("  1. Use ChangeAddress example with the known current address");
    Serial.println("  2. Check sensor documentation for address restore procedure");
  }
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}
