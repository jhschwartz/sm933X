// SM9333.cpp
#include "SM9333.h"
#include <Wire.h>

SM9333:SM9333(int multiplex_output) {
    this.multiplex_output = multiplex_output;
    assert this.isConnected();
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
    int command[3] = {0x2E, 0x5B, 0xDB};
    commandSequence seq = {command, 3};
    writer(seq);
    int* result = doRead(7, true);
    int temp_low_bit = result[0];
    int temp_high_bit = result[1];
    int pres_low_bit = result[2];
    int pres_high_bit = result[3];

    pressureTemperaturePair pair = {
        calcPressure(pres_low_bit, pres_high_bit),
        calcTemperature(temp_low_bit, temp_high_bit)
    }
    return pair;
}

double SM9333::calcPressure(int pressureLowBit, int pressureHighBit) {
    int digital = pressureLowBit | pressureHighBit << 8;
    double p_pc = 100.0/(pow(2,16)-1)*double(digital) + 100.0*pow(2,16)/(2.0*(pow(2,16)-1));
    // the datasheet calculation of p_p appears to be a mistake. -P_MIN instead of +. 
    double p_p = (P_MIN-P_MAX)/80.0 * p_pc - P_MIN - (P_MIN-P_MAX)/8.0;
    return p_p;
}

double SM9333::calcTemperature(int temperatureLowBit, int temperatureHighBit) {
    int digital = temperatureLowBit | temperatureHighBit << 8;
    double temp = (double(digital) - b0)/b1;
    return temp;
}

void SM9333::writer(commandSequence seq) {
    this.multiplex_switch();
    Wire.beginTransmission(address_unprotected);
    for (int i = 0; i < seq.length; i++) {
        Wire.write(byte(seq.sequence[i]));
    }
    Wire.endTransmission();
}

int* SM9333::doRead(int numBits, bool crcProtected, int location) {
    this.multiplex_switch();
    int crcLoc = crcProtected ? address_protected : address_unprotected;
    int command[3] = {location, 0x5B, 0xDB};
    commandSequence seq = {command, 3};
    writer(seq);
    Wire.requestFrom(crcLoc, numBits);

    int result[numBits];
    for (int i = 0; i < numBits; i++) {
        result[i] = Wire.read();
    }
    return result;
}

void SM9333::multiplex_switch() {
    Wire.beginTransmission(0x70);
    Wire.write(this.multiplex_output);
    Wire.endTransmission();
}
