/*!
 * @file UseChangedAddress.ino
 * 
 * @brief Example demonstrating how to use a sensor with a changed I2C address
 * 
 * This sketch shows how to use the DYP-R01CW laser ranging sensor after its
 * I2C address has been changed from the default. This example assumes you have
 * already run the ChangeAddress example to change the sensor's address to 0xD4.
 * 
 * If you need to change the address first, run the ChangeAddress example before
 * running this one. If you need to restore the sensor to the default address,
 * use the RestoreAddress example.
 * 
 * This example uses the same address (0xD4) that the ChangeAddress example sets,
 * demonstrating how to work with a sensor at a non-default address.
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
 * @section prerequisites Prerequisites
 * 
 * Before running this example, you must have already changed the sensor's
 * address using the ChangeAddress example. The sensor should be configured
 * to address 0xD4 (8-bit format).
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

// Changed I2C address (8-bit format)
// This matches the address set by the ChangeAddress example
#define CHANGED_ADDRESS_8BIT 0xD4

// Create sensor object with the changed address
DYP_R01CW sensor(CHANGED_ADDRESS_8BIT);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Use Changed Address Example");
  Serial.println("=============================================================");
  Serial.println();
  
  Serial.print("This example uses sensor at address 0x");
  Serial.print(CHANGED_ADDRESS_8BIT, HEX);
  Serial.println(" (8-bit format)");
  Serial.println();
  
  Serial.println("IMPORTANT: Before running this example, you must:");
  Serial.println("  1. Run the ChangeAddress example to change the sensor address to 0xD4");
  Serial.println("  2. Ensure the sensor is powered and properly connected");
  Serial.println();
  
  // Initialize the sensor at the changed address
  Serial.print("Initializing sensor at changed address 0x");
  Serial.print(CHANGED_ADDRESS_8BIT, HEX);
  Serial.println("...");
  
  if (!sensor.begin()) {
    Serial.println("ERROR: Could not find DYP-R01CW sensor at changed address!");
    Serial.println();
    Serial.println("Please check:");
    Serial.println("  1. Wiring connections");
    Serial.println("  2. Sensor has been configured to address 0xD4 using ChangeAddress example");
    Serial.println("  3. Sensor is powered on with 3.3-5.0V");
    Serial.println();
    Serial.println("If the sensor is still at the default address (0xE8):");
    Serial.println("  - Run the ChangeAddress example first");
    Serial.println();
    Serial.println("If you want to restore the sensor to default address (0xE8):");
    Serial.println("  - Run the RestoreAddress example");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("Sensor found and initialized successfully!");
  Serial.println();
  
  // Read and display software version
  uint16_t version = sensor.readSoftwareVersion();
  if (version != 0) {
    Serial.print("Software Version: 0x");
    Serial.println(version, HEX);
  } else {
    Serial.println("WARNING: Could not read software version");
  }
  
  Serial.println();
  Serial.println("Starting continuous distance measurements...");
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
