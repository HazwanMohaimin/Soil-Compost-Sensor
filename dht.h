#include <DHT.h>
#define  DHTTYPE DHT22

#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);
String tempData, humidData;

void dhtData(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  humidData = String(h);
  tempData = String(t);

  if(isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT22!");
    return;
    delay(1000);
  }

  delay(3000);
}
