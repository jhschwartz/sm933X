#include "SM9333.h"

SM9333 sensor1;
SM9333 sensor2;
SM9333 sensor3;
SM9333 sensor4;
SM9333 sensor5;

void setup() {
  Serial.begin(9600);

  sensor1.set_multiplex(0x71);
  sensor2.set_multiplex(0x72);
  sensor3.set_multiplex(0x73);
  sensor4.set_multiplex(0x74);
  sensor5.set_multiplex(0x75);

}

void loop() {
  double p1 = sensor1.readPressure();
  double p2 = sensor2.readPressure();
  double p3 = sensor2.readPressure();
  double p4 = sensor2.readPressure();
  double p5 = sensor2.readPressure();

  Serial.println("Sensor 1: " + (String)p1 + " Pa");
  Serial.println("Sensor 2: " + (String)p2 + " Pa");
  Serial.println("Sensor 3: " + (String)p3 + " Pa");
  Serial.println("Sensor 4: " + (String)p4 + " Pa");
  Serial.println("Sensor 5: " + (String)p5 + " Pa");

  Serial.println("------------------------");

  delay(200);

}
