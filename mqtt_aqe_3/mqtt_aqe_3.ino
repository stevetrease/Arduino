
//    

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
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

char message_buff[30];

#define MQTT_SERVER "192.168.1.103"
byte MAC_ADDRESS[] = { 0x91, 0xA3, 0xBB, 0x00, 0x01, 0x03 };
EthernetClient ethClient;  // Ethernet object
PubSubClient client( MQTT_SERVER, 1883, callback, ethClient); // MQTT object




 
void setup() {
  // set random MaAC 
  randomSeed(analogRead(0));  
  for (byte i = 1; i < 6; ++i) {
    MAC_ADDRESS[i] = random(0,255);     
  } 
 
  Serial.begin(57600);
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LOW);        // off  
  
  Serial.println("Initialising...");
  // flash the LED to signify booting
  digitalWrite(LEDpin, HIGH);
  delay(100);   
  digitalWrite(LEDpin, LOW);
  
  Serial.println("DHCP...");
  if (Ethernet.begin(MAC_ADDRESS) == 0)       // Get connected!
  {
      Serial.println("Failed to configure Ethernet using DHCP");
      return;
  } else {
      Serial.print("IP: ");                       // A little debug.. show IP address
      Serial.println(Ethernet.localIP());
  }
 
  if (client.connect("arduinoClient")) {      // connect to MQTT server
    Serial.println("Connected to MQTT");      // let us know this has happened
  }

  
  eggBus.init();
  egg_bus_address = eggBus.next();
    
  Serial.print("Egg Bus Address: 0x");
  Serial.println(egg_bus_address, HEX);
  Serial.print("Firmware Version: ");
  Serial.println(eggBus.getFirmwareVersion(), DEC);
  Serial.println();
}  
  

void loop() { 
  // temperature
  Serial.print("Temp:\t");
  dtostrf(getTemperature(), 4, 2, message_buff);
  Serial.println(message_buff);
  client.publish("sensors/temperature/aqe", message_buff);      
  

  // humidity
  Serial.print("Humi:\t");
  dtostrf(getHumidity(), 4, 2, message_buff);
  Serial.println(message_buff);
  client.publish("sensors/humidity/aqe", message_buff);      


  // CO
  dtostrf(readEggSensor(egg_bus_address, 1), 4, 12, message_buff);
  Serial.print("CO:\t");
  Serial.println(message_buff);
  client.publish("sensors/co/aqe", message_buff);      


  // NO2
  dtostrf(readEggSensor(egg_bus_address, 0), 4, 12, message_buff);
  Serial.print("NO2:\t");
  Serial.println(message_buff);
  client.publish("sensors/no2/aqe", message_buff);      

  
  digitalWrite(LEDpin, HIGH);
  delay(75);   
  digitalWrite(LEDpin, LOW);

  delay(random(5000,10000));
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
  
//    Serial.println("===========================");

//   Serial.print("Sensor Index: ");
//   Serial.print(ii, DEC);       
      
//   Serial.print("    Sensor Type: ");
//    Serial.print(eggBus.getSensorType(ii));
     
//      Serial.print("   Indep. Scaler: ");
    i_scaler = eggBus.getIndependentScaler(ii);
//     Serial.println(i_scaler, 8);
      
//      Serial.print("  Table X Scaler: ");
      x_scaler = eggBus.getTableXScaler(ii);
//    Serial.println(x_scaler, 8);     
     
//      Serial.print("  Table Y Scaler: ");
      y_scaler = eggBus.getTableYScaler(ii);
//      Serial.println(y_scaler, 8);         

//      Serial.print(" Measured Value: ");
      measured_value = eggBus.getSensorIndependentVariableMeasure(ii);
//      Serial.print(measured_value, DEC);        
//      Serial.print(" => ");
//      Serial.println(measured_value * i_scaler, 8);

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
      
//      Serial.print("    Sensor Value: ");
//     Serial.print(eggBus.getSensorValue(ii), DEC);    
      
//      Serial.print("    Sensor Units: ");
//      Serial.println(eggBus.getSensorUnits(ii));  
      return ((float)eggBus.getSensorValue(ii));     
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



void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}
