
//    


#include <stdint.h>
#include <DHT.h>
#include "Wire.h"
#include "EggBus.h"

int LEDpin   = 9;

#define DHTPIN 17
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

EggBus eggBus;
uint8_t   egg_bus_address;


 
void setup() {
  Serial.begin(57600);
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);        // off  
  
  Serial.println("Initialising...");
  // flash the LED to signify booting
  digitalWrite(LEDpin, HIGH);
  delay(100);   
  digitalWrite(LEDpin, HIGH);
  
  eggBus.init();
  egg_bus_address = eggBus.next();
    
  Serial.print("Egg Bus Address: 0x");
  Serial.println(egg_bus_address, HEX);
  Serial.print("Firmware Version: ");
  Serial.println(eggBus.getFirmwareVersion(), DEC);
  Serial.println();
}  
  

void loop() { 

  data.type = 'A';
  data.data = (float) getTemperature();
  Serial.print(data.type);
  Serial.println(data.data);  

 
  data.type = 'B';
  data.data = (float) getHumidity();
  Serial.print(data.type);
  Serial.println(data.data);

  
  data.type = 'C';
  data.data = (float) readEggSensor(egg_bus_address, 1);
  Serial.print(data.type);
  Serial.println(data.data);


  data.type = 'D';
  data.data = (float) readEggSensor(egg_bus_address, 0);
  Serial.print(data.type);
  Serial.println(data.data);

}




long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}



float readEggSensor(uint8_t egg_bus_address, int ii) {
   float x_scaler = 0.0;
   float y_scaler = 0.0;
   float i_scaler = 0.0;
   uint32_t measured_value = 0;
   uint32_t r0 = 0;
  
   Serial.println("===========================");

   Serial.println("---------------------------");
   Serial.print("  Sensor Index: ");
   Serial.println(ii, DEC);       
      
   Serial.print("    Sensor Type: ");
   Serial.println(eggBus.getSensorType(ii));
     
//      Serial.print("   Indep. Scaler: ");
    i_scaler = eggBus.getIndependentScaler(ii);
//     Serial.println(i_scaler, 8);
      
//      Serial.print("  Table X Scaler: ");
      x_scaler = eggBus.getTableXScaler(ii);
//    Serial.println(x_scaler, 8);     
     
//      Serial.print("  Table Y Scaler: ");
      y_scaler = eggBus.getTableYScaler(ii);
//      Serial.println(y_scaler, 8);         

      Serial.print(" Measured Value: ");
      measured_value = eggBus.getSensorIndependentVariableMeasure(ii);
      Serial.print(measured_value, DEC);        
      Serial.print(" => ");
      Serial.println(measured_value * i_scaler, 8);

//      Serial.print("              R0: ");
      r0 = eggBus.getSensorR0(ii);
//      Serial.println(r0);
      
//      Serial.print("Implied Resistance: ");
      if(measured_value == 0xffffffff){
        Serial.println("OPEN CIRCUIT");
      }
//      else{
//        Serial.println(measured_value * i_scaler * r0, 8);
//      }
      
      uint8_t xval, yval, row = 0;
//      while(eggBus.getTableRow(ii, row++, &xval, &yval)){
//        Serial.print("     Table Row ");
//        Serial.print(row);
//        Serial.print(": [");
//        Serial.print(xval, DEC);
//        Serial.print(", ");        
//        Serial.print(yval, DEC);        
//        Serial.print("] => [");
//        Serial.print(x_scaler * xval, 8);
//        Serial.print(", ");
//        Serial.print(y_scaler * yval, 8);
//        Serial.println("]");
//      }
      
      Serial.print("    Sensor Value: ");
      Serial.println(eggBus.getSensorValue(ii), DEC);    
      
      Serial.print("    Sensor Units: ");
      Serial.println(eggBus.getSensorUnits(ii));  
      return (eggBus.getSensorValue(ii));     
}




void sensorSetup(){
  //Serial.println("--- DHT22 BEGIN ---");
  dht.begin(); 
  delay(500);
}

//--------- DHT22 humidity ---------
float getHumidity(){
  float h = dht.readHumidity();
  if (isnan(h)){
    //failed to get reading from DHT    
    delay(2500);
    h = dht.readHumidity();
    if(isnan(h)){
      return -1; 
    }
  } 
  return h;
}

//--------- DHT22 temperature ---------
float getTemperature(){
  float t = dht.readTemperature();
  if (isnan(t)){
    //failed to get reading from DHT
    delay(2500);
    t = dht.readTemperature();
    if(isnan(t)){
      return -1; 
    }
  } 
  return t;
}
