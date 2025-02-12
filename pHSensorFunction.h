#ifndef PHSENSOR_H
#define PHSENSOR_H

#include <Arduino.h>

#ifdef USE_PULSE_OUT
  #include "ph_iso_survey.h"
  // If using pulse out, initialize with isolated pH sensor
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A3);  // Use A3 for pH sensor pin
#else
  #include "ph_surveyor.h"
  // If not using pulse out, initialize the standard pH sensor with a pin
  Surveyor_pH pH = Surveyor_pH(A3);  // Use A3 for pH sensor pin
#endif

// Function to initialize the pH sensor
void init_pHSensor() {
  Serial.begin(9600);
  delay(200);
  Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin()) {
    Serial.println("Loaded EEPROM");
  }
}

// Function to read pH value
float read_pH() {
  return pH.read_ph();
}

// Function to parse and execute calibration commands
void parse_pHCommand(const char* command) {
  if (strcmp(command, "CAL,7") == 0) {
    pH.cal_mid();
    Serial.println("MID CALIBRATED");
  } else if (strcmp(command, "CAL,4") == 0) {
    pH.cal_low();
    Serial.println("LOW CALIBRATED");
  } else if (strcmp(command, "CAL,10") == 0) {
    pH.cal_high();
    Serial.println("HIGH CALIBRATED");
  } else if (strcmp(command, "CAL,CLEAR") == 0) {
    pH.cal_clear();
    Serial.println("CALIBRATION CLEARED");
  }
}

#endif  // This closes the #ifndef PHSENSOR_H block
