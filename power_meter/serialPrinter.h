/**
 * @file
 * @brief       Handles the printing of data to the serial port.
 *
 * @author      Björn Johansson
 * @date        2025-01-03
 * @version     1.0
 *
 * @license     MIT
 *   This project is licensed under the MIT License - see the LICENSE file for details.
 *
 * @details
 *
 */

#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include <Arduino.h>

class SerialPrinter
{
public:
    /**
     * @brief       Constructor for the SerialPrinter class.
     * @param       serail The serial stream to print to.
     */
    SerialPrinter(Serial_ &serial);

    /**
     * @brief       Destructor for the SerialPrinter class.
     */
    ~SerialPrinter();

    /**
     * @brief       Initialize the serial printer.
     * @param       baudrate The baudrate to use.
     */
    void begin(int baudrate = 115200);

    /**
     * @brief       Print a new line.
     */
    void println();

    /**
     * @brief       Print the message with a new line
     * @param       message The message to print.
     */
    void println(String& msg);

    /**
     * @brief       Print the message with a new line
     * @param       message The message to print.
     */
    void println(const char *message);

    /**
     * @brief       Print the message without a newline.
     * @param       message The message to print.
     */
    void print(const char *message);

    /**
     * @brief       Print the number without a new line.
     * @param       number The number to print.
     */
    void print(unsigned long number);

    /**
     * @brief       Print the number without a new line.
     * @param       number The number to print.
     * @param       precision The number of decimal places to print.
     */
    void print(float number, int precision);

private:

    /* Keep the default constructor private and undefined
    */
    SerialPrinter();

    /**
     * @brief       The serial stream to print to.
     */
    Serial_ &m_serial;

    /*! True if the serial port has been activated.
    */
    bool m_bSerialActivated = false;
};


#endif //  SERIALPRINTER_H