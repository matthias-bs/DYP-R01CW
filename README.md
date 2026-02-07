# DYP-R01CW

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
| VCC        | 5V (or 3.3V depending on sensor version) |
| GND        | GND         |
| SDA        | SDA (A4 on Uno, 20 on Mega) |
| SCL        | SCL (A5 on Uno, 21 on Mega) |

## Usage

### Basic Example

```cpp
#include <Wire.h>
#include <DYP_R01CW.h>

// Create sensor object with default I2C address (0x70, which is 0xE0 in 8-bit format)
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

## API Reference

### Constructor

```cpp
DYP_R01CW(uint8_t addr = DYP_R01CW_DEFAULT_ADDR)
```

Creates a new DYP_R01CW sensor object.

- `addr`: I2C address of the sensor (default: 0x50)

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
- **Note:** The sensor's default 8-bit address is 0xE0 (7-bit: 0x70), which is now the library default
- **Note:** The new address takes effect immediately and the object's internal address is updated automatically. You can continue using the same sensor object without creating a new one.

**Example:**

```cpp
// Change sensor address from default 0x70 (7-bit) to 0xD4 (8-bit = 0x6A in 7-bit)
DYP_R01CW sensor;  // Uses default address 0x70
sensor.begin();

if (sensor.setAddress(0xD4)) {
  Serial.println("Address changed successfully!");
  // No need to create a new object - the same object now uses the new address
  int16_t distance = sensor.readDistance();  // Works with new address
} else {
  Serial.println("Failed to change address");
}
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
