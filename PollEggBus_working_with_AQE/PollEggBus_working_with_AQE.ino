#include <stdint.h>

#include "Wire.h"
#include "EggBus.h"

EggBus eggBus;

void setup(){
  uint8_t   egg_bus_address;

  Serial.begin(57600);
  
  eggBus.init();
  egg_bus_address = eggBus.next();
  Serial.println("===========================");

  Serial.print("Firmware Version: ");
  Serial.println(eggBus.getFirmwareVersion(), DEC);
  Serial.println("===========================");
}

void loop(){
  uint8_t   egg_bus_address;
  float x_scaler = 0.0;
  float y_scaler = 0.0;
  float i_scaler = 0.0;
  uint32_t measured_value = 0;
  uint32_t r0 = 0;
  
 
  
  eggBus.init();
    egg_bus_address = eggBus.next();
    Serial.println("===========================");
    // Serial.print("Egg Bus Address: 0x");
    // Serial.println(egg_bus_address, HEX);

    // Serial.print("Firmware Version: ");
    // Serial.println(eggBus.getFirmwareVersion(), DEC);

    // Serial.print("  Sensor Address: ");
    // printAddress(eggBus.getSensorAddress());             
    uint8_t numSensors = eggBus.getNumSensors();
    for(uint8_t ii = 0; ii < numSensors; ii++){
    
      // Serial.println("---------------------------");
      // Serial.print("  Sensor Index: ");
      // Serial.println(ii, DEC);       
      
      // Serial.print("    Sensor Type: ");
      Serial.print(eggBus.getSensorType(ii));
     
      // Serial.print("   Indep. Scaler: ");
      // i_scaler = eggBus.getIndependentScaler(ii);
      // Serial.println(i_scaler, 8);
      
      // Serial.print("  Table X Scaler: ");
      // x_scaler = eggBus.getTableXScaler(ii);
      // Serial.println(x_scaler, 8);     
     
      // Serial.print("  Table Y Scaler: ");
      // y_scaler = eggBus.getTableYScaler(ii);
      // Serial.println(y_scaler, 8);         

      // Serial.print(" Measured Value: ");
      // measured_value = eggBus.getSensorIndependentVariableMeasure(ii);
      // Serial.print(measured_value, DEC);        
      // Serial.print(" => ");
      // Serial.println(measured_value * i_scaler, 8);

      // Serial.print("              R0: ");
      // r0 = eggBus.getSensorR0(ii);
      // Serial.println(r0);
      
      // Serial.print("Implied Resistance: ");
      // if(measured_value == 0xffffffff){
        // Serial.println("OPEN CIRCUIT");
      // }
      // else{
        // Serial.println(measured_value * i_scaler * r0, 8);
      // }
      
      // uint8_t xval, yval, row = 0;
      // while(eggBus.getTableRow(ii, row++, &xval, &yval)){
       // Serial.print("     Table Row ");
        // Serial.print(row);
        // Serial.print(": [");
        // Serial.print(xval, DEC);
        // Serial.print(", ");        
        // Serial.print(yval, DEC);        
        // Serial.print("] => [");
        // Serial.print(x_scaler * xval, 8);
        // Serial.print(", ");
        // Serial.print(y_scaler * yval, 8);
        // Serial.println("]");
      // }
      
      Serial.print(": ");
      Serial.print(eggBus.getSensorValue(ii), DEC);    
      
      Serial.print(" ");
      Serial.println(eggBus.getSensorUnits(ii));            
    }
  
  delay(5000);
}

void printAddress(uint8_t * address){
  for(uint8_t jj = 0; jj < 6; jj++){
    if(address[jj] < 16) Serial.print("0");
    Serial.print(address[jj], HEX);
    if(jj != 5 ) Serial.print(":");
  }
  Serial.println();
}
