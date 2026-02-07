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

// Create sensor object with default I2C address (0x50)
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

## License

MIT License - see LICENSE file for details

## Author

Matthias Prinke

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
