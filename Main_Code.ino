#include <OneWire.h>
#include <DallasTemperature.h>
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Soil Moisture Sensor
#define SOIL_PIN A7
#define WET_SOIL 277
#define DRY_SOIL 380

// DS18B20 Temperature Sensor
#define ONE_WIRE_BUS 31
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// RS485 NPK Sensor
#define RE 8
#define DE 9

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];

// pH Sensor
#define PH_PIN A8
float voltage, phValue, temperature = 25;
DFRobot_PH ph;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27

// Helper for Modbus response
bool readModbusResponse(byte* buffer, byte length) {
  unsigned long start = millis();
  byte index = 0;
  while (index < length && (millis() - start < 1000)) {
    if (Serial2.available()) {
      buffer[index++] = Serial2.read();
    }
  }
  return index == length;
}

// NPK functions
byte readNutrient(const byte* command) {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  Serial2.write(command, 8);
  Serial2.flush();

  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  if (readModbusResponse(values, 7)) {
    return values[4];
  } else {
    return 0; // Error or timeout
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600); // Using Serial2 for RS485

  sensors.begin();
  ph.begin();

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  digitalWrite(RE, LOW);
  digitalWrite(DE, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Compost Monitor");
  delay(2000);
  lcd.clear();

  // Optional: single-time pH calibration (comment out if not needed)
  voltage = analogRead(PH_PIN) / 1024.0 * 5000;
  ph.calibration(voltage, 25);
}

void loop() {
  // ---- Soil Moisture ----
  int moisture = analogRead(SOIL_PIN);
  Serial.print("Soil Moisture: ");
  Serial.println(moisture);

  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  if (moisture < WET_SOIL) {
    lcd.print("Too Wet     ");
    Serial.println("Too Wet");
  } else if (moisture < DRY_SOIL) {
    lcd.print("Perfect     ");
    Serial.println("Perfect");
  } else {
    lcd.print("Too Dry     ");
    Serial.println("Too Dry");
  }
  delay(3000);

  // ---- Temperature ----
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" C");

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC, 1);
  lcd.print(" C     ");
  delay(3000);

  // ---- NPK ----
  uint8_t nitrogenVal    = readNutrient(nitro);
  delay(100);
  uint8_t phosphorousVal = readNutrient(phos);
  delay(100);
  uint8_t potassiumVal   = readNutrient(pota);
  delay(100);

  Serial.print("Nitrogen: ");
  Serial.print(nitrogenVal);
  Serial.println(" mg/kg");

  Serial.print("Phosphorous: ");
  Serial.print(phosphorousVal);
  Serial.println(" mg/kg");

  Serial.print("Potassium: ");
  Serial.print(potassiumVal);
  Serial.println(" mg/kg");

  lcd.setCursor(0, 0);
  lcd.print("N:");
  lcd.print(nitrogenVal);
  lcd.print(" P:");
  lcd.print(phosphorousVal);
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("K:");
  lcd.print(potassiumVal);
  delay(2000);

  // ---- pH ----
  voltage = analogRead(PH_PIN) / 1024.0 * 5000;
  phValue = ph.readPH(voltage, tempC);
  Serial.print("pH: ");
  Serial.println(phValue, 2);

  lcd.setCursor(0, 0);
  lcd.print("pH Level:      ");
  lcd.setCursor(0, 1);
  lcd.print(phValue, 2);
  lcd.print("         ");
  delay(3000);

  Serial.println("------------------------------");
}
