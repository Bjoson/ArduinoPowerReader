# Power meter reader

This project implements a power meter reader for home power meters which flashes a led for each unit of power consumed.
It can also control a garage door by short circuiting the motor control connections.
The project connects to Wifi so that the readings can be accessed remotely

## Power meter

Most power meters for houses has a flashing led which flashes for some unit of power conusmed. The code in this project has been created for a Kamstrup single-phase residential meter. This meter emits pulses at 1000 pulses per kWh. The pulse time is 30 ms.


## http interface
 * get is called to get the latest power readings
 * door is called to trigger the garage door motor

