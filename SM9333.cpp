// SM9333.cpp
#include "SM9333.h"
#include <Wire.h>
#include <assert.h>
#include <math.h>

SM9333::SM9333() {}

void SM9333::set_multiplex(int multiplex_output) {
    if (multiplex_output < 0x71 || multiplex_output > 0x77)
        assert(false);
    this->multiplex_output = multiplex_output;
    assert(this->isConnected());
}

bool SM9333::isConnected() {
    // do an actual check that the address can be reached??
    return true;
}

double SM9333::readPressure() {
    pressureTemperaturePair pair = readBoth();
    return pair.pressure;
}

double SM9333::readTemperature() {
    pressureTemperaturePair pair = readBoth();
    return pair.temperature;
}

pressureTemperaturePair SM9333::readBoth() {
    // int command[3] = {0x2E, 0x5B, 0xDB};
    // commandSequence seq = {command, 3};
    // writer(seq);
    int* result = doRead(7, true, 0x2E);
    int temp_low_bit = result[0];
    int temp_high_bit = result[1];
    int pres_low_bit = result[2];
    int pres_high_bit = result[3];

    pressureTemperaturePair pair = {
        calculatePressure(pres_low_bit, pres_high_bit),
        calculateTemperature(temp_low_bit, temp_high_bit)
    };

    return pair;
}

double SM9333::calculatePressure(int pressureLowByte, int pressureHighByte) {
    int digital = pressureLowByte | pressureHighByte << 8;
    double p_pc = 100.0/(pow(2,16)-1)*double(digital) + 100.0*pow(2,16)/(2.0*(pow(2,16)-1));
    // the datasheet calculation of p_p appears to be a mistake. -P_MIN instead of +. 
    double p_p = (P_MIN-P_MAX)/80.0 * p_pc - P_MIN - (P_MIN-P_MAX)/8.0;
    return p_p;
}

double SM9333::calculateTemperature(int temperatureLowBit, int temperatureHighBit) {
    int digital = temperatureLowBit | temperatureHighBit << 8;
    double temp = (double(digital) - b0)/b1;
    return temp;
}

void SM9333::writer(commandSequence seq) {
    this->multiplex_switch();
    Wire.beginTransmission(SM9333_UNPROTECTED);
    for (int i = 0; i < seq.length; i++) {
        int val = seq.sequence[i];
        // Wire.write((byte)val);
        Wire.write(val);
    }
    Wire.endTransmission();
}

int* SM9333::doRead(int numBits, bool crcProtected, int location) {
    this->multiplex_switch();
    int crcLoc = crcProtected ? SM9333_CRC_PROTECTED : SM9333_UNPROTECTED;
    int command[3] = {location, 0x5B, 0xDB};
    commandSequence seq = {command, 3};
    writer(seq);
    Wire.requestFrom(crcLoc, numBits);

    // int result[numBits];
    int* result;
    for (int i = 0; i < numBits; i++) {
        *(result+i) = Wire.read();
    }
    return result;
}

void SM9333::multiplex_switch() {
    Wire.beginTransmission(0x70);
    Wire.write(this->multiplex_output);
    Wire.endTransmission();
}
