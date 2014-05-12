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


// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }


char  topic[STRING_BUFFER];
char  message[STRING_BUFFER];

// Payload data structure
typedef struct {		
      	  int            nodeid;		// sending node ID
          char           type;            
	  float          data;		
} Payload;
Payload data;


 
void setup() {
  byte macaddr[6];
  NanodeUNIO unio(NANODE_MAC_DEVICE);
  
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
  Serial.println("1");
  mqtt.connect();
  Serial.println("2");
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  Serial.println("3");
}  
  

void loop() {

  uip.poll();
  Serial.print(".");
  
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
     
      sprintf (topic, "nanode/sensor/%d", data.nodeid);
      dtostrf(data.data, 4, 2, message);
      
      // Serial.print(topic);
      // Serial.print(", ");
      // Serial.println(message);
 
  
      
        
      if (mqtt.connected()) {
        Serial.print("Publishing ");
        Serial.print(topic);
        Serial.print(":");
        Serial.println(message);

        mqtt.publish(topic, message);
        mqtt.publish("Test", "Hello World!");
        Serial.println("Published.");

        // quick flash on publish
        digitalWrite(RED_LED, LOW);
        delay(50);              
        digitalWrite(RED_LED, HIGH);  
      }
    }
  }
}

