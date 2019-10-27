// SM9333.h
#ifndef SM9333_H
#define SM9333_H

#define P_MIN -125
#define P_MAX 125

class SM9333
{
public:
    SM9333();
    bool connect();
    bool is_valid();
    double read_pressure();
    double read_temperature();

private:
    int address;

    double calculate_pressure(int pressure_low_byte, int pressure_high_byte);
    double calculate_temperature(int temperature_low_byte, int temperature_high_byte);
    bool next_read_ready();
    void writer();


};







#endif