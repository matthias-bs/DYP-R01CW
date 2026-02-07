/*!
 * @file DYP_R01CW.cpp
 * 
 * DYP-R01CW / DFRobot SEN0590 Laser Ranging Sensor Library for Arduino
 * 
 * @section author Author
 * 
 * Written by Matthias Prinke
 * 
 * @section license License
 * 
 * MIT License
 */

#include "DYP_R01CW.h"

/*!
 * @brief Constructor
 * @param addr I2C address of the sensor
 */
DYP_R01CW::DYP_R01CW(uint8_t addr) {
    _addr = addr;
    _wire = nullptr;
}

/*!
 * @brief Initialize the sensor and I2C communication
 * @param wire Pointer to TwoWire object
 * @return true if initialization successful, false otherwise
 */
bool DYP_R01CW::begin(TwoWire *wire) {
    _wire = wire;
    
    // Initialize I2C if not already initialized
    if (_wire == &Wire) {
        _wire->begin();
    }
    
    // Check if sensor is connected
    return isConnected();
}

/*!
 * @brief Read distance measurement from the sensor
 * @return Distance in millimeters, or -1 if read failed
 */
int16_t DYP_R01CW::readDistance() {
    if (_wire == nullptr) {
        return -1;
    }
    
    // Request 2 bytes from sensor
    uint8_t bytesReceived = _wire->requestFrom(_addr, (uint8_t)2);
    
    if (bytesReceived != 2) {
        return -1;
    }
    
    // Read high and low bytes
    uint8_t highByte = _wire->read();
    uint8_t lowByte = _wire->read();
    
    // Combine bytes to form distance value
    int16_t distance = (highByte << 8) | lowByte;
    
    return distance;
}

/*!
 * @brief Check if sensor is connected and responding
 * @return true if sensor is connected, false otherwise
 */
bool DYP_R01CW::isConnected() {
    if (_wire == nullptr) {
        return false;
    }
    
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    
    return (error == 0);
}
