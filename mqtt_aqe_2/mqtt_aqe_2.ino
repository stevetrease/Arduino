
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>
#include "EggBus.h"


#define MQTT_SERVER "192.168.1.103"

#define DHTPIN 17 // analog pin 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

byte MAC_ADDRESS[] = { 0x91, 0xA3, 0xBB, 0x00, 0x01, 0x03 };
char topic_buff[30];
char message_buff[30];

EggBus eggBus;


int LEDpin   = 9;   

EthernetClient ethClient;  // Ethernet object
PubSubClient client( MQTT_SERVER, 1883, callback, ethClient); // MQTT object

void setup() {
  // set random MaAC 
  for (byte i = 1; i < 6; ++i) {
    MAC_ADDRESS[i] = random(0,255);     
  } 
  
  Serial.begin(57600);
  randomSeed(analogRead(0));  

  Serial.println("DHCP...");
  
  pinMode(LEDpin, OUTPUT);

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
}
    





void loop(){
  uint8_t   egg_bus_address;
  float x_scaler = 0.0;
  float y_scaler = 0.0;
  float i_scaler = 0.0;
  uint32_t measured_value = 0;
  uint32_t r0 = 0;
  
  eggBus.init();
  digitalWrite(LEDpin, HIGH);
  while((egg_bus_address = eggBus.next())){
    Serial.println("===========================");
    Serial.print("Egg Bus Address: 0x");
    Serial.println(egg_bus_address, HEX);

    Serial.print("Firmware Version: ");
    Serial.println(eggBus.getFirmwareVersion(), DEC);
  
    uint8_t numSensors = eggBus.getNumSensors();
    for(uint8_t ii = 0; ii < numSensors; ii++){
    
      Serial.println("---------------------------");
      Serial.print("  Sensor Index: ");
      Serial.println(ii, DEC);       
      
      Serial.print("    Sensor Type: ");
      Serial.println(eggBus.getSensorType(ii));
     
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
      
      Serial.print("    Sensor Value: ");
      Serial.println(eggBus.getSensorValue(ii), DEC);    
      
      Serial.print("    Sensor Units: ");
      Serial.println(eggBus.getSensorUnits(ii));   
     
      float number = (eggBus.getSensorValue(ii));
      dtostrf(number, 4, 7, message_buff);
      
      String sensor = String(eggBus.getSensorType(ii));
      sensor.toLowerCase();
      String topicString = "sensors/" + sensor + "/aqe";
      topicString.toCharArray(topic_buff, topicString.length()+1);
      client.publish(topic_buff, message_buff);      
    }
  }
    
 
  float number = getHumidity();
  dtostrf(number, 4, 2, message_buff);
  client.publish("sensors/humidity/aqe", message_buff);
  
  number = getTemperature();
  dtostrf(number, 4, 2, message_buff);
  client.publish("sensors/temperature/aqe", message_buff);
  
  digitalWrite(LEDpin, LOW);
  delay(random(5000,10000));
}




// handles message arrived on subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {     
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
  else return h;
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
