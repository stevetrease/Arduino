//declare variables
float tempC, tempC0, tempC1;
int tempPin0 = 0;
int tempPin1 = 1;

void setup()
{
Serial.begin(57600); //opens serial port, sets data rate to 9600 bps
}

void loop()
{
  tempC = analogRead(tempPin0);           //read the value from the sensor
  tempC0 = (5.0 * tempC * 100.0)/1024.0;  //convert the analog data to temperature
  Serial.print(tempC0);  //send the data to the computer

  Serial.print("\t");
  
  tempC = analogRead(tempPin1);           //read the value from the sensor
  tempC1 = (5.0 * tempC * 100.0)/1024.0;  //convert the analog data to temperature
  Serial.print(tempC1);                   //send the data to the computer

  Serial.print("\t");
  Serial.print(tempC0 - tempC1);

  Serial.println();
  delay(1000);                           //wait one second before sending new data
}
