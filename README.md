# DYP-R01CW
[![CI](https://github.com/matthias-bs/DYP-R01CW/actions/workflows/CI.yml/badge.svg)](https://github.com/matthias-bs/BDYP-R01CW/actions/workflows/CI.yml)
[![GitHub release](https://img.shields.io/github/release/matthias-bs/DYP-R01CW?maxAge=3600)](https://github.com/matthias-bs/DYP-R01CW/releases)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://github.com/matthias-bs/DYP-R01CW/blob/main/LICENSE)

DYP-R01CW / DFRobot SEN0590 Laser Ranging Sensor Library for Arduino

## Description

An Arduino library for interfacing with the DYP-R01CW (DFRobot SEN0590) laser ranging sensor via I2C (Wire interface). This library provides a simple API for reading distance measurements from the sensor.

## Features

- Simple I2C interface using Arduino Wire library
- Distance measurement in millimeters
- Connection checking functionality
- Configurable I2C address support
- Compatible with all Arduino boards that support the Wire library

## Installation

### Arduino IDE

#### Via Library Manager

1. Open the Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries (or Tools → Manage Libraries)
3. Search for "DYP-R01CW"
4. Click "Install"

#### From ZIP File

1. Download this repository as a ZIP file
2. In the Arduino IDE, go to Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file
4. The library will be installed and ready to use

### PlatformIO

Add the following to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/matthias-bs/DYP-R01CW
```

## Hardware Specifications

The DYP-R01CW sensor has the following electrical specifications:

| Parameter | Value | Notes |
|-----------|-------|-------|
| Supply Voltage | 3.3V - 5.0V | Compatible with both 3.3V and 5V logic |
| Communication Rate | 1 - 100 kbit/s | I2C bus speed |
| Recommended Pull-up Resistors | 3 - 10 kΩ | For I2C SDA and SCL lines |

## Hardware Connection

Connect the DYP-R01CW sensor to your Arduino board:

| Sensor Pin | Arduino Pin |
|------------|-------------|
| VCC        | Supply voltage (3.3V - 5.0V) |
| GND        | GND         |
| SDA        | SDA (A4 on Uno, 20 on Mega) |
| SCL        | SCL (A5 on Uno, 21 on Mega) |

## Usage

### Basic Example

```cpp
#include <Wire.h>
#include <DYP_R01CW.h>

// Create sensor object with default I2C address (0xE8 in 8-bit format)
DYP_R01CW sensor;

void setup() {
  Serial.begin(115200);
  
  // Initialize the sensor
  if (!sensor.begin()) {
    Serial.println("ERROR: Could not find sensor!");
    while (1);
  }
  
  Serial.println("Sensor initialized!");
}

void loop() {
  // Read distance from sensor
  int16_t distance = sensor.readDistance();
  
  if (distance >= 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("ERROR: Failed to read distance");
  }
  
  delay(500);
}
```

### Custom I2C Pins Example

For ESP32/ESP8266 boards, you can use custom GPIO pins for I2C communication and set a custom clock frequency:

```cpp
#include <Wire.h>
#include <DYP_R01CW.h>

// Custom I2C pins
#define CUSTOM_SDA_PIN 25
#define CUSTOM_SCL_PIN 26

// Custom I2C clock frequency (10000 Hz = 10 kHz)
#define CUSTOM_I2C_CLOCK_FREQ 10000

DYP_R01CW sensor;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C with custom pins (ESP32/ESP8266 specific)
  Wire.begin(CUSTOM_SDA_PIN, CUSTOM_SCL_PIN);
  
  // Set custom I2C clock frequency
  Wire.setClock(CUSTOM_I2C_CLOCK_FREQ);
  
  // Initialize the sensor (pass &Wire to use the configured Wire instance)
  if (!sensor.begin(&Wire)) {
    Serial.println("ERROR: Could not find sensor!");
    while (1);
  }
  
  Serial.println("Sensor initialized!");
}

void loop() {
  int16_t distance = sensor.readDistance();
  
  if (distance >= 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("ERROR: Failed to read distance");
  }
  
  delay(500);
}
```

**Note:** Custom I2C pins are supported on ESP32 and ESP8266. For other Arduino boards (e.g., Uno, Mega), use the default hardware I2C pins.

**Tip:** A reduced clock frequency (like 10 kHz shown above) can help improve reliability when using long wires or in noisy EMC (Electromagnetic Compatibility) environments by reducing signal integrity issues.

## API Reference

### Constructor

```cpp
DYP_R01CW(uint8_t addr = DYP_R01CW_DEFAULT_ADDR)
```

Creates a new DYP_R01CW sensor object.

- `addr`: I2C address of the sensor in 8-bit format (default: 0xE8)

### Methods

#### begin()

```cpp
bool begin(TwoWire *wire = &Wire)
```

Initializes the sensor and I2C communication.

- `wire`: Pointer to TwoWire object (default: &Wire)
- Returns: `true` if initialization successful, `false` otherwise

#### readDistance()

```cpp
int16_t readDistance()
```

Reads distance measurement from the sensor.

- Returns: Distance in millimeters, or -1 if read failed

#### isConnected()

```cpp
bool isConnected()
```

Checks if sensor is connected and responding.

- Returns: `true` if sensor is connected, `false` otherwise

#### readSoftwareVersion()

```cpp
uint16_t readSoftwareVersion()
```

Reads the software version number from the sensor.

- Returns: Software version number as a 16-bit value (two bytes from register 0x00), or 0 if read failed

#### setAddress()

```cpp
bool setAddress(uint8_t newAddr)
```

Sets the I2C address of the sensor by writing to the slave address register (0x05).

- `newAddr`: New I2C address in 8-bit format - must be one of the 20 supported addresses
- **Supported addresses (8-bit):** 0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF8, 0xFA, 0xFC, 0xFE
  - Even addresses from 0xD0-0xFE, excluding reserved range 0xF0-0xF6
- Returns: `true` if address was set successfully, `false` otherwise
- **Important:** The `newAddr` parameter uses 8-bit I2C address format (includes R/W bit)
- **Note:** The sensor's default 8-bit address is 0xE8
- **Note:** The new address takes effect immediately and the object's internal address is updated automatically. You can continue using the same sensor object without creating a new one.
- **Note:** The sensor stores the new address persistently.

**Example:**

```cpp
// Change sensor address from default 0xE8 (8-bit) to 0xD4 (8-bit)
DYP_R01CW sensor;  // Uses default address 0xE8
sensor.begin();

if (sensor.setAddress(0xD4)) {
  Serial.println("Address changed successfully!");
  // No need to create a new object - the same object now uses the new address
  int16_t distance = sensor.readDistance();  // Works with new address
} else {
  Serial.println("Failed to change address");
}
```

#### setDistanceOffset()

```cpp
void setDistanceOffset(int16_t offset)
```

Sets the distance offset to be applied to all distance readings.

- `offset`: Offset in millimeters to add to distance readings (can be positive or negative)
- **Note:** The default offset is 0 mm
- **Use case:** Calibrate the sensor or compensate for mounting distance

**Example:**

```cpp
DYP_R01CW sensor;
sensor.begin();

// Apply a positive offset (sensor reads 10mm shorter than actual)
sensor.setDistanceOffset(10);

// Apply a negative offset (sensor reads 5mm longer than actual)
sensor.setDistanceOffset(-5);

// Reset to no offset
sensor.setDistanceOffset(0);
```

#### getDistanceOffset()

```cpp
int16_t getDistanceOffset()
```

Gets the current distance offset.

- Returns: Current offset in millimeters

**Example:**

```cpp
DYP_R01CW sensor;
sensor.begin();

sensor.setDistanceOffset(10);
int16_t currentOffset = sensor.getDistanceOffset();  // Returns 10
Serial.print("Current offset: ");
Serial.print(currentOffset);
Serial.println(" mm");
```

## Related Resources

- **[DYP-R01CW Product Page](https://www.dypcn.com/small-size-waterproof-laser-sensor-dyp-r01-product/)** - Official product page from DYP with technical specifications and product details
- **[DFRobot SEN0590 Wiki](https://wiki.dfrobot.com/Laser_Ranging_Sensor_4m_SKU_SEN0590)** - DFRobot's wiki documentation for the SEN0590 laser ranging sensor (compatible module)
- **[ESPHome Integration](https://github.com/sredfern/sen0590_esphome)** - ESPHome custom component for the SEN0590/DYP-R01CW sensor by sredfern

## License

MIT License - see LICENSE file for details

## Author

Matthias Prinke

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
