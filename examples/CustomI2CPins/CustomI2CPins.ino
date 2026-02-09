/*!
 * @file CustomI2CPins.ino
 * 
 * @brief Example for DYP-R01CW laser ranging sensor with custom I2C pins and datarate
 * 
 * This sketch demonstrates how to use the DYP-R01CW library with custom I2C pins
 * and a custom I2C datarate. This is particularly useful for ESP32/ESP8266 boards
 * where you may want to use different GPIO pins for I2C communication.
 * 
 * Custom Configuration:
 * - SDA Pin: 25
 * - SCL Pin: 26
 * - I2C Datarate: 10000 bits/s (10 kbit/s)
 * 
 * @section hardware Hardware Requirements
 * 
 * - ESP32 or ESP8266 board (or other Arduino boards supporting custom I2C pins)
 * - DYP-R01CW / DFRobot SEN0590 laser ranging sensor
 * - I2C connection:
 *   - SDA to GPIO 25
 *   - SCL to GPIO 26
 *   - VCC to 5V (or 3.3V depending on sensor version)
 *   - GND to GND
 * 
 * @section notes Notes
 * 
 * - This example uses custom I2C pins which is supported on ESP32 and ESP8266.
 * - For other Arduino boards (e.g., Uno, Mega), custom pins may not be supported,
 *   and you should use the default hardware I2C pins.
 * - The custom datarate of 10000 bits/s is lower than the typical 100000 bits/s,
 *   which can be useful for longer cable runs or noisy environments.
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

// Custom I2C pins
#define CUSTOM_SDA_PIN 25
#define CUSTOM_SCL_PIN 26

// Custom I2C datarate (10000 bits/s = 10 kbit/s)
#define CUSTOM_I2C_DATARATE 10000

// Create sensor object with default I2C address (0xE8 in 8-bit format)
DYP_R01CW sensor;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Custom I2C Pins Example");
  Serial.println("==========================================================");
  Serial.print("Custom SDA Pin: ");
  Serial.println(CUSTOM_SDA_PIN);
  Serial.print("Custom SCL Pin: ");
  Serial.println(CUSTOM_SCL_PIN);
  Serial.print("Custom I2C Datarate: ");
  Serial.print(CUSTOM_I2C_DATARATE);
  Serial.println(" bits/s");
  Serial.println();
  
#if defined(ESP32) || defined(ESP8266)
  // Initialize I2C with custom pins (ESP32/ESP8266 specific)
  Wire.begin(CUSTOM_SDA_PIN, CUSTOM_SCL_PIN);
  
  // Set custom I2C clock speed
  Wire.setClock(CUSTOM_I2C_DATARATE);
  
  Serial.println("Custom I2C pins and datarate configured for ESP32/ESP8266");
#else
  // For other boards, use default I2C pins but set custom clock speed
  Wire.begin();
  Wire.setClock(CUSTOM_I2C_DATARATE);
  
  Serial.println("WARNING: Custom I2C pins are not supported on this board.");
  Serial.println("Using default hardware I2C pins with custom datarate.");
#endif
  
  Serial.println();
  
  // Initialize the sensor (pass &Wire to use the already configured Wire instance)
  if (!sensor.begin(&Wire)) {
    Serial.println("ERROR: Could not find DYP-R01CW sensor!");
    Serial.println("Please check wiring and I2C address.");
    Serial.println("Make sure the sensor is connected to the custom pins:");
    Serial.print("  SDA: GPIO ");
    Serial.println(CUSTOM_SDA_PIN);
    Serial.print("  SCL: GPIO ");
    Serial.println(CUSTOM_SCL_PIN);
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
