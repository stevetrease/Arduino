#include <JeeLib.h>
#include <NanodeUNIO.h>
#include <NanodeUIP.h>
#include <NanodeMQTT.h>

NanodeMQTT mqtt(&uip);

#define STRING_BUFFER 64

// RF stuff
#define RF_NODEID 1        // master node
#define RF_CHANNEL 125

#define GREEN_LED 5
#define RED_LED 6

#define ON LOW
#define OFF HIGH


// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }


char  topic[STRING_BUFFER];
char  message[STRING_BUFFER];

// Payload data structure
typedef struct {    
          int            nodeid;    // sending node ID
          char           type;            
          float          data;    
} Payload;
Payload data;


 
void setup() {
  byte macaddr[6];
  NanodeUNIO unio(NANODE_MAC_DEVICE);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, OFF); 
  digitalWrite(GREEN_LED, ON); 
  
  Serial.begin(57600);
  Serial.println("Initialising...");

  unio.read(macaddr, NANODE_MAC_ADDRESS, 6);
  uip.init(macaddr);

  // FIXME: use DHCP instead
  uip.set_ip_addr(192, 168, 1, 198 );
  uip.set_netmask(255, 255, 255, 0);

  uip.wait_for_link();
  Serial.println("Link is up");

  // FIXME: resolve using DNS instead
  mqtt.set_server_addr(192, 168, 1, 103);
  mqtt.connect();
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  mqtt.publish("test", "nanode booted");
  digitalWrite(GREEN_LED, OFF);
}  
  

void loop() {

  uip.poll();
  // Serial.print(".");
  
  if (rf12_recvDone()) {
    if (rf12_crc == 0) {
      Payload temp = *(Payload*) rf12_data;
      data = temp;
      
      Serial.print("n:");
      Serial.print(data.nodeid);
      Serial.print("\tt:");
      Serial.print(data.type);
      Serial.print("\td:");
      Serial.print(data.data);     
      Serial.println();
     
      sprintf (topic, "nanode/sensor/%d/%c", data.nodeid, data.type);
      dtostrf(data.data, 10, 8, message);

      if (mqtt.connected()) {
        digitalWrite(GREEN_LED, OFF);
        digitalWrite(RED_LED, ON);
        
        Serial.print("Publishing ");
        Serial.print(topic);
        Serial.print(" : ");
        Serial.println(message);

        mqtt.publish(topic, message);
             
        digitalWrite(RED_LED, OFF);  
      } else {
        digitalWrite(RED_LED, ON);   
        digitalWrite(GREEN_LED, ON); 

        Serial.println("not connected");
      }
    }
  }
}
