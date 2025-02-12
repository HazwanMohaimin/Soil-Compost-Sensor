//Define Analog input
#define sensorPin A0

void moist(){
  
  // Read the analog input
  int value = analogRead(sensorPin);

  // Print the value to the serial monitor
  Serial.print("Analog output: ");
  Serial.println(value);

  // Wait for 1 second before the next reading
  delay(1000);
}
