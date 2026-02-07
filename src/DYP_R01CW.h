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

// Default I2C address for DYP-R01CW sensor (7-bit format)
// Sensor's default 8-bit address is 0xE0, which equals 0x70 in 7-bit format
#define DYP_R01CW_DEFAULT_ADDR 0x70

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
     * @param newAddr New I2C address in 8-bit format
     * @return true if address was set successfully, false otherwise
     * @note The new address takes effect immediately and the object's internal address
     *       is updated automatically. You can continue using the same object.
     * @note Supported 8-bit addresses (20 total): 0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA,
     *       0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF8, 0xFA,
     *       0xFC, 0xFE (even addresses from 0xD0-0xFE, excluding 0xF0-0xF6)
     * @note The sensor's default 8-bit address is 0xE0 (7-bit: 0x70)
     * @note To convert: 7-bit address = 8-bit address >> 1, or 8-bit address = 7-bit address << 1
     */
    bool setAddress(uint8_t newAddr);

private:
    uint8_t _addr;        ///< I2C address of the sensor
    TwoWire *_wire;       ///< Pointer to Wire object
};

#endif // DYP_R01CW_H
