#include "serialPrinter.h"

SerialPrinter::SerialPrinter(Serial_ &serial) :
    m_serial(serial)
{
    // Nothing to do
}

SerialPrinter::~SerialPrinter()
{
    // Nothing to do
}

void SerialPrinter::begin(int baudrate)
{
    m_serial.begin(baudrate);

    unsigned long startAttemptTimeMs = millis();
    //Allow serial to initialize, up to 2000ms
    while (!m_serial)
    {
        if ((millis() - startAttemptTimeMs) > 2000)
        {
            break;
        }
    }

    if (m_serial)
    {
        m_bSerialActivated = true;
    }
}

void SerialPrinter::println()
{
    if (m_bSerialActivated)
    {
        m_serial.println();
    }
}

void SerialPrinter::println(const char *message)
{
    if (m_bSerialActivated)
    {
        m_serial.println(message);
    }
}

void SerialPrinter::print(const char *message)
{
    if (m_bSerialActivated)
    {
        m_serial.print(message);
    }
}

void SerialPrinter::print(unsigned long number)
{
    if (m_bSerialActivated)
    {
        m_serial.print(number);
    }
}

void SerialPrinter::print(float number, int precision)
{
    if (m_bSerialActivated)
    {
        m_serial.print(number, precision);
    }
}

void SerialPrinter::println(String& msg)
{
    if (m_bSerialActivated)
    {
        m_serial.println(msg);
    }
}