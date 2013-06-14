
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>


#define MQTT_SERVER "192.168.1.103"

#define DHTPIN 17 // analog pin 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

byte MAC_ADDRESS[] = { 0x91, 0xA3, 0xBB, 0x00, 0x01, 0x03 };
char message_buff[30];

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

void loop()
{
  digitalWrite(LEDpin, HIGH);
  
  // String pubString;
 
  float number = getHumidity();
  dtostrf(number, 4, 2, message_buff);
  // pubString.toCharArray(message_buff, pubString.length()+1);
  client.publish("sensors/humidity/aqe", message_buff);
  
  number = getTemperature();
  dtostrf(number, 4, 2, message_buff);
  client.publish("sensors/temperature/aqe", message_buff);
 
  digitalWrite(LEDpin, LOW);  
  delay(random(10000,20000));
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
