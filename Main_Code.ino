#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "dht.h"
#include "npkFunction.h"
#include "pHSensorFunction.h"
#include "soilMoisture.h"


// Enter the I2C address and the dimensions of your LCD here
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD Address


void setup() {

  // put your set code here, to run once;
  dht.begin();
  Serial.begin(9600);
  modbus.begin(9600);

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  //Lcd Setup
  lcd.init();
  lcd.clear();
  lcd.backlight(); // Make sure blacklight is on

  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0); // Set the cursor to character 2 on lines
  lcd.print("Soil Compost");
  
  lcd.setCursor(2,1); // Move cursor to character 2 on line 1
  lcd.print("Innovaero sdn bh");

  init_pHSensor(); // initlaizer the ph Sensor

}

void loop() {
  
  // put your main code here, to run repeadtedly:

  // Displaying "Done by Innovaero"
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Done BY");
  lcd.setCursor(3,1);
  lcd.print("INNOVAERO");
  delay(2000);

  dhtData();
  lcd.clear();

  //Temperature reading sensor
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempData);
  lcd.print(" ");
  lcd.write(0xDF);
  lcd.print( "C" );

  // Humid reading sensor
  lcd.setCursor(0,1);
  lcd.print("Humid: ");
  lcd.print(humidData);
  lcd.print("%");
  delay(2000);

  lcd.clear();

  // NPK configuration Display
  byte val1, val2, val3;

  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);

  nitro_st = String(val1);
  phos_st = String(val2);
  pota_st = String(val3);

  //Nitrogen reading NPK
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("N: ");
  lcd.print(val2);

  //Potassium reading npk
  lcd.setCursor(0,1); // column inde, row index
  lcd.print("K: ");
  lcd.print(val3);
  lcd.setCursor(7,1); //Column index,row index
  lcd.print("mg/kg");
  delay(2000);

  // Read user input for calibration commands
  if (Serial.available() > 0) {
    char user_data[32];
    size_t user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
    if (user_bytes_received > 0) {
      user_data[user_bytes_received] = '\0'; // Null-terminate the string
      parse_pHCommand(user_data);
    }
  }

  // Display pH value on the LCD
  float pHValue = read_pH();
  lcd.clear();
    lcd.setCursor(0, 0);
  lcd.print("pH: ");
  lcd.print(pHValue);
  delay(2000);

}
