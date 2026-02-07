/*!
 * @file DYP_R01CW.h
 * 
 * DYP-R01CW / DFRobot SEN0590 Laser Ranging Sensor Library for Arduino
 * 
 * @section intro_sec Introduction
 * 
 * This library provides a simple interface for the DYP-R01CW laser ranging sensor
 * using the Wire (I2C) interface.
 * 
 * @section author Author
 * 
 * Written by Matthias Prinke
 * 
 * @section license License
 * 
 * MIT License
 */

#ifndef DYP_R01CW_H
#define DYP_R01CW_H

#include <Arduino.h>
#include <Wire.h>

// Default I2C address for DYP-R01CW sensor
#define DYP_R01CW_DEFAULT_ADDR 0x50

// Sensor registers
#define DYP_R01CW_VERSION_REG 0x00
#define DYP_R01CW_COMMAND_REG 0x10
#define DYP_R01CW_DATA_REG 0x02
#define DYP_R01CW_SLAVE_ADDR_REG 0x05

// Commands
#define DYP_R01CW_MEASURE_COMMAND 0xB0

/*!
 * @brief DYP_R01CW class for interfacing with the laser ranging sensor
 */
class DYP_R01CW {
public:
    /*!
     * @brief Constructor for DYP_R01CW
     * @param addr I2C address of the sensor (default: 0x50)
     */
    DYP_R01CW(uint8_t addr = DYP_R01CW_DEFAULT_ADDR);
    
    /*!
     * @brief Initialize the sensor
     * @param wire Pointer to TwoWire object (default: &Wire)
     * @return true if initialization successful, false otherwise
     */
    bool begin(TwoWire *wire = &Wire);
    
    /*!
     * @brief Read distance measurement from the sensor
     * @return Distance in millimeters, or -1 if read failed
     */
    int16_t readDistance();
    
    /*!
     * @brief Check if sensor is connected and responding
     * @return true if sensor is connected, false otherwise
     */
    bool isConnected();
    
    /*!
     * @brief Read software version number from the sensor
     * @return Software version number (16-bit), or 0 if read failed
     */
    uint16_t readSoftwareVersion();
    
    /*!
     * @brief Set the I2C address of the sensor
     * @param newAddr New I2C address (must be one of: 0xD0, 0xD2, 0xD4, ..., 0xFE)
     * @return true if address was set successfully, false otherwise
     * @note The new address takes effect immediately. After calling this method,
     *       you must create a new sensor object with the new address to communicate
     *       with the sensor.
     * @note Only even addresses from 0xD0 to 0xFE are valid (20 addresses total)
     * @note Default address is 0xE0
     */
    bool setAddress(uint8_t newAddr);

private:
    uint8_t _addr;        ///< I2C address of the sensor
    TwoWire *_wire;       ///< Pointer to Wire object
};

#endif // DYP_R01CW_H
