#include <Wire.h>

//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Simple code for the TMP102, simply prints temperature via serial
//////////////////////////////////////////////////////////////////

#include <Wire.h>
int tmp102Address_1 = 0x48;
int tmp102Address_2 = 0x49;
byte res;
int val;

void setup(){        
  Serial.begin(9600);
  Wire.begin();
  pinMode(9, OUTPUT); 
}

void loop(){
  Wire.requestFrom(tmp102Address_1,2); 
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
  float celsius = TemperatureSum*0.0625;  
  Serial.print("Celsius 1: ");
  Serial.println(celsius);

  Wire.requestFrom(tmp102Address_2,2); 
  MSB = Wire.read();
  LSB = Wire.read();
  TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
  celsius = TemperatureSum*0.0625; 
  Serial.print("Celsius 2: ");
  Serial.println(celsius);

  digitalWrite(9, HIGH);   // set the LED on
  delay(250);              // wait
  digitalWrite(9, LOW);    // set the LED off
  delay(1000);
}
