// SM9333.h
#ifndef SM9333_H
#define SM9333_H

#define P_MIN -125
#define P_MAX 125

#define SM9333_UNPROTECTED 0x6C
#define SM9333_CRC_PROTECTED 0x6D

class SM9333
{
public:
    // consructor
    SM9333();

    // connect the chip & sets self.address
    // Wiring must be correct and board must be connected.
    // Returns if connect was successful
    bool connect();

    // checks if the chip is connected
    bool isConnected();

    // Returns a pressure reading
    double readPressure();

    // Returns a temperature reading
    double readTemperature();

    struct pressureTemperaturePair {
        double pressure;
        double temperature;
    }

    struct commandSequence {
        int* sequence;
        int length;
    }

    // Returns pressure and temperature readings (they're always sent by the chip at the same time)
    pressureTemperaturePair readBoth();

private:
    // the address of the instance/the physical chip
    int address;

    // using the low byte and high byte sent by the chip, calculates the Pressure using bitwise math and
    // also the equation provided in the datasheet.
    double calculatePressure(int pressureLowBit, int pressureHighBit);

    // using the low Bit and high Bit sent by the chip, calculates the Temperature using bitwise math and
    // also the equation provided in the datasheet.
    double calculateTemperature(int temperatureLowBit, int temperatureHighBit);

    // used to write a sequence of bits to the chip
    void writer(commandSequence seq);

    // requests a read from the chip of numBits number of bits, and with or without CRC protection
    int* doRead(int numBits, bool crcProtected, int location);

};


#endif