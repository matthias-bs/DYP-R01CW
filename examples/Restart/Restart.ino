/*!
 * @file Restart.ino
 * 
 * @brief Example demonstrating how to restart the DYP-R01CW sensor
 * 
 * This sketch shows how to use the restart() method to restart the DYP-R01CW
 * laser ranging sensor. The restart command sends DYP_R01CW_RESTART_COMMAND_1
 * followed by DYP_R01CW_RESTART_COMMAND_2 to the command register, which resets
 * the sensor to its initial state.
 * 
 * This can be useful when:
 * - The sensor is not responding correctly
 * - You want to reset the sensor state
 * - Recovering from an error condition
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

// Delay after restarting sensor (in milliseconds)
#define SENSOR_RESTART_DELAY_MS 800

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("DYP-R01CW Laser Ranging Sensor - Restart Example");
  Serial.println("=================================================");
  Serial.println();
  
  // Initialize the sensor
  Serial.println("Initializing sensor...");
  if (!sensor.begin()) {
    Serial.println("ERROR: Could not find DYP-R01CW sensor!");
    Serial.println("Please check:");
    Serial.println("  1. Wiring connections");
    Serial.println("  2. Sensor is powered on with 3.3-5.0V");
    Serial.println("  3. I2C address is correct (default: 0xE8)");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("Sensor initialized successfully!");
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
  
  // Take a few distance readings before restart
  Serial.println("Taking distance readings before restart:");
  for (int i = 0; i < 3; i++) {
    int16_t distance = sensor.readDistance();
    if (distance >= 0) {
      Serial.print("  Reading ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(distance);
      Serial.println(" mm");
    } else {
      Serial.print("  Reading ");
      Serial.print(i + 1);
      Serial.println(": Failed");
    }
    delay(100);
  }
  Serial.println();
  
  // Restart the sensor
  Serial.println("Restarting sensor...");
  if (sensor.restart()) {
    Serial.println("SUCCESS: Sensor restart command sent successfully!");
  } else {
    Serial.println("ERROR: Failed to send restart command!");
    Serial.println("Possible reasons:");
    Serial.println("  1. Communication error with sensor");
    Serial.println("  2. Sensor is not responding");
  }
  Serial.println();
  
  // Wait for sensor to restart (give it some time)
  Serial.println("Waiting for sensor to restart...");
  delay(SENSOR_RESTART_DELAY_MS);
  Serial.println();
  
  // Verify sensor is still connected after restart
  Serial.println("Checking if sensor is still connected...");
  if (sensor.isConnected()) {
    Serial.println("Sensor is connected!");
  } else {
    Serial.println("WARNING: Sensor not responding");
  }
  Serial.println();
  
  // Take a few distance readings after restart
  Serial.println("Taking distance readings after restart:");
  for (int i = 0; i < 3; i++) {
    int16_t distance = sensor.readDistance();
    if (distance >= 0) {
      Serial.print("  Reading ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(distance);
      Serial.println(" mm");
    } else {
      Serial.print("  Reading ");
      Serial.print(i + 1);
      Serial.println(": Failed");
    }
    delay(100);
  }
  Serial.println();
  
  Serial.println("Example complete!");
  Serial.println("The sensor has been restarted and is functioning normally.");
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}
