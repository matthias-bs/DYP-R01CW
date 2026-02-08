/*!
 * @file ChangeAddress.ino
 *
 * @brief Example demonstrating how to change the I2C address of DYP-R01CW sensor
 *
 * This sketch shows how to change the I2C address of the DYP-R01CW laser ranging
 * sensor. The sensor supports 20 different 8-bit addresses: 0xD0, 0xD2, 0xD4, 0xD6,
 * 0xD8, 0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF8,
 * 0xFA, 0xFC, 0xFE (even addresses from 0xD0-0xFE, excluding 0xF0-0xF6).
 *
 * The sensor's default 8-bit address is 0xE8.
 *
 * IMPORTANT: The constructor and setAddress() methods now both use 8-bit address format.
 *
 * IMPORTANT: After calling setAddress(), the sensor object automatically updates
 * to use the new address. You can continue using the same object.
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

// Use default I2C address (0xE8 in 8-bit format)
// If your sensor has been configured to a different address, change this value

// New I2C address to set (must be one of the supported 8-bit addresses)
// This example changes to 0xD4 (8-bit)
#define NEW_ADDRESS_8BIT 0xD4

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  // Wait a moment to allow the serial monitor to connect before printing
  delay(2000);
#else
  while (!Serial)
  {
    ; // Wait for serial port to connect (needed for native USB)
  }
#endif

  Serial.println("DYP-R01CW Laser Ranging Sensor - Change Address Example");
  Serial.println("========================================================");
  Serial.println();

  // Create sensor object with default address (0xE8 in 8-bit format)
  DYP_R01CW sensor;

  // Initialize the sensor
  Serial.println("Initializing sensor at default address 0xE8 (8-bit)...");

  if (!sensor.begin())
  {
    Serial.println("ERROR: Could not find DYP-R01CW sensor!");
    Serial.println("Please check:");
    Serial.println("  1. Wiring connections");
    Serial.println("  2. Sensor is at the default address (0xE8)");
    Serial.println("  3. Sensor is powered on with 3.3-5.0V");
    while (1)
    {
      delay(1000);
    }
  }

  Serial.println("Sensor found!");
  Serial.println();

  // Read current software version
  uint16_t version = sensor.readSoftwareVersion();
  if (version != 0)
  {
    Serial.print("Software Version: 0x");
    Serial.println(version, HEX);
  }
  Serial.println();

  // List supported addresses
  Serial.println("Supported 8-bit addresses (20 total):");
  Serial.println("0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,");
  Serial.println("0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,");
  Serial.println("0xF8, 0xFA, 0xFC, 0xFE");
  Serial.println("(Note: 0xF0-0xF6 are reserved and not supported)");
  Serial.println();

  // Change the address
  Serial.print("Changing sensor address to 0x");
  Serial.print(NEW_ADDRESS_8BIT, HEX);
  Serial.println(" (8-bit format)...");

  if (sensor.setAddress(NEW_ADDRESS_8BIT))
  {
    Serial.println("SUCCESS: Address changed successfully!");
    Serial.println("The sensor object has been automatically updated to use the new address.");
    Serial.println();

    // Wait for sensor to process the address change (experimentally determined, not documented by manufacturer)
    delay(800);

    // Test reading distance with the same sensor object (now using new address)
    Serial.println("Testing distance measurement with new address:");
    int16_t distance = sensor.readDistance();
    if (distance >= 0)
    {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" mm");
    }
    else
    {
      Serial.println("Could not read distance (this is normal if nothing is in range)");
    }
  }
  else
  {
    Serial.println("ERROR: Failed to change address!");
    Serial.println("Possible reasons:");
    Serial.println("  1. Invalid address (must be one of the 20 supported addresses)");
    Serial.println("  2. Communication error with sensor");
    Serial.println("  3. Sensor does not support address change");
  }

  Serial.println();
  Serial.println("Example complete.");
}

void loop()
{
  // Nothing to do in loop
  delay(1000);
}
