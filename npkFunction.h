#include <Adafruit_I2CDevice.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

//modbus to arduino connection
#define DI 9
#define RO 8
#define RE 7
#define DE 6

//const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[]  = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[]  = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];
//SoftwareSerial modbus(2,3);
SoftwareSerial modbus(RO, DI);
String nitro_st, phos_st, pota_st;

byte nitrogen(){
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if(modbus.write(nitro, sizeof(nitro)) == 8){
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for(byte i=0; i<7; i++){
    //Serial.print(mod.read(), HEX);
    values[i] = modbus.read();
    Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte phosphorous(){
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);

  delay(10);
  if(modbus.write(phos, sizeof(phos)) == 8){
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for(byte i=0; i<7; i++){
      //Serial.print(mod.read(),HEX);
      values[i]  = modbus.read();
      Serial.println(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte potassium(){
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  if(modbus.write(pota, sizeof(pota)) == 8){
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for(byte i=0; i<7; i++){
      //Serial.print(mod.read(), HEX;
      values[i] = modbus.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

void npkValue(){
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
}
