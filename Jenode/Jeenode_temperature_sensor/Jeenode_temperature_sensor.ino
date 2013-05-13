#include <JeeLib.h>
#include <Ports.h>


//    TMP102 on Jeenode port 1
//      SDA to DIO
//      SCL to AIO

int tmp102Address = 0x48;  // ADD0 to ground

PortI2C   myport(1, PortI2C::KHZ100);
DeviceI2C temperature(myport, tmp102Address);  



void setup() {
  Serial.begin(57600);
  
  
}

void loop() {
  float temp_C = getTemperature();
 
  // Serial.print(millis()/1000.0);
  // Serial.print("\t");
  // Serial.println(temp_C);

  delay(2500); //just here to slow down the output. You can remove this
}


float getTemperature()
{   
  temperature.send();             // go into send mode
  temperature.write(0x00);        //
  temperature.receive();          // grab it
  byte MSB = temperature.read(0); // Whole degrees
  // temperature.send();             // go into send mode
  // temperature.write(0x10);        
  // temperature.receive();          // grab it
  byte LSB = temperature.read(1); // fractional degrees
  temperature.stop();             // done

  Serial.print(MSB,BIN);
  Serial.print(" ");
  Serial.print(LSB,BIN);
  Serial.print("\t");
  
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
  
  Serial.print(TemperatureSum, BIN);
  Serial.print("\t");
  float celsius = TemperatureSum*0.0625; 
  
  Serial.print(celsius);
  Serial.println();
  
  return celsius;
}
