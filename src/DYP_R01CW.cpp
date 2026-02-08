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
 * @param addr I2C address of the sensor in 8-bit format
 */
DYP_R01CW::DYP_R01CW(uint8_t addr) {
    // Convert 8-bit address to 7-bit format for Wire library
    _addr = addr >> 1;
    _wire = nullptr;
    _distanceOffset = 0;  // Default offset is 0
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
    
    // Check if sensor is responding by reading the software version
    uint16_t version = readSoftwareVersion();
    
    // Version of 0 indicates a communication error
    if (version == 0) {
        return false;
    }
    
    return true;
}

/*!
 * @brief Read distance measurement from the sensor
 * @return Distance in millimeters, or -1 if read failed
 */
int16_t DYP_R01CW::readDistance() {
    if (_wire == nullptr) {
        return -1;
    }
    
    // Send measurement command to command register
    _wire->beginTransmission(_addr);
    _wire->write(DYP_R01CW_COMMAND_REG);
    _wire->write(DYP_R01CW_MEASURE_COMMAND);
    uint8_t error = _wire->endTransmission();
    
    if (error != 0) {
        return -1;
    }
    
    // Wait for measurement to complete (sensor requires ~50ms)
    delay(50);
    
    // Set pointer to data register
    _wire->beginTransmission(_addr);
    _wire->write(DYP_R01CW_DATA_REG);
    error = _wire->endTransmission();
    
    if (error != 0) {
        return -1;
    }
    
    // Request 2 bytes from data register
    uint8_t bytesReceived = _wire->requestFrom(_addr, (uint8_t)2);
    
    if (bytesReceived != 2) {
        return -1;
    }
    
    // Read high and low bytes
    uint8_t highByte = _wire->read();
    uint8_t lowByte = _wire->read();
    
    // Combine bytes to form distance value
    uint16_t rawDistance = (highByte << 8) | lowByte;
    
    // Check for invalid data
    if (rawDistance == 0xFFFF) {
        return -1;
    }
    
    // Apply offset and return distance in millimeters
    int16_t distance = rawDistance + _distanceOffset;
    
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

/*!
 * @brief Read software version number from the sensor
 * @return Software version number (16-bit), or 0 if read failed
 */
uint16_t DYP_R01CW::readSoftwareVersion() {
    if (_wire == nullptr) {
        return 0;
    }
    
    // Set pointer to version register
    _wire->beginTransmission(_addr);
    _wire->write(DYP_R01CW_VERSION_REG);
    uint8_t error = _wire->endTransmission();
    
    if (error != 0) {
        return 0;
    }
    
    // Request 2 bytes from version register
    uint8_t bytesReceived = _wire->requestFrom(_addr, (uint8_t)2);
    
    if (bytesReceived != 2) {
        return 0;
    }
    
    // Read high and low bytes
    uint8_t highByte = _wire->read();
    uint8_t lowByte = _wire->read();
    
    // Combine bytes to form version number
    uint16_t version = (highByte << 8) | lowByte;
    
    return version;
}

/*!
 * @brief Set the I2C address of the sensor
 * @param newAddr New I2C address in 8-bit format (must be one of: 0xD0, 0xD2, 0xD4, ..., 0xFE)
 * @return true if address was set successfully, false otherwise
 */
bool DYP_R01CW::setAddress(uint8_t newAddr) {
    if (_wire == nullptr) {
        return false;
    }
    
    // Validate the new address
    // Valid addresses are 0xD0, 0xD2, ..., 0xEE, 0xF8, 0xFA, 0xFC, 0xFE (20 addresses)
    // These are even addresses from 0xD0 to 0xFE, excluding 0xF0-0xF6
    if (newAddr < 0xD0 || newAddr > 0xFE || (newAddr & 0x01) != 0) {
        return false;
    }
    // Exclude reserved addresses 0xF0, 0xF2, 0xF4, 0xF6
    if (newAddr >= 0xF0 && newAddr <= 0xF6) {
        return false;
    }
    
    // Write the new address to the slave address register
    _wire->beginTransmission(_addr);
    _wire->write(DYP_R01CW_SLAVE_ADDR_REG);
    _wire->write(newAddr);
    uint8_t error = _wire->endTransmission();
    
    if (error != 0) {
        return false;
    }
    
    // Address change successful: update internal 7-bit address
    _addr = newAddr >> 1;
    
    return true;
}

/*!
 * @brief Set the distance offset
 * @param offset Offset in millimeters to add to distance readings (can be positive or negative)
 */
void DYP_R01CW::setDistanceOffset(int16_t offset) {
    _distanceOffset = offset;
}

/*!
 * @brief Get the current distance offset
 * @return Current offset in millimeters
 */
int16_t DYP_R01CW::getDistanceOffset() {
    return _distanceOffset;
}

/*!
 * @brief Restart the sensor
 * @return true if restart command was sent successfully, false otherwise
 */
bool DYP_R01CW::restart() {
    if (_wire == nullptr) {
        return false;
    }
    
    // Send restart command sequence to command register
    _wire->beginTransmission(_addr);
    _wire->write(DYP_R01CW_COMMAND_REG);
    _wire->write(DYP_R01CW_RESTART_COMMAND_1);
    _wire->write(DYP_R01CW_RESTART_COMMAND_2);
    uint8_t error = _wire->endTransmission();
    
    return (error == 0);
}
