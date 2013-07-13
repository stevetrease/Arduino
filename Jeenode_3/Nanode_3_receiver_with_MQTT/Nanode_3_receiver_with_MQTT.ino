#include <JeeLib.h>
#include <NanodeUNIO.h>
#include <NanodeUIP.h>
#include <NanodeMQTT.h>

NanodeMQTT mqtt(&uip);
struct timer my_timer;

#define STRING_BUFFER 128

// RF stuff
#define RF_NODEID 1        // master node
#define RF_CHANNEL 125

#define GREEN_LED 5
#define RED_LED 6


// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }

unsigned int count_crc = 0;
unsigned int count_pkts = 0;

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

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);        //off  

  Serial.begin(57600);
  Serial.println("Initialising...");

  unio.read(macaddr, NANODE_MAC_ADDRESS, 6);
  uip.init(macaddr);

  // FIXME: use DHCP instead
  uip.set_ip_addr(192, 168, 1, 199);
  uip.set_netmask(255, 255, 255, 0);

  uip.wait_for_link();
  Serial.println("Link is up");

  // FIXME: resolve using DNS instead
  mqtt.set_server_addr(192, 168, 1, 103);
  mqtt.connect();
 
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  
  // flash the LED to signify booting
  digitalWrite(GREEN_LED, LOW);
  delay(100);              
  digitalWrite(GREEN_LED, HIGH);
  delay(150); 
  digitalWrite(GREEN_LED, LOW);
  delay(200);              
  digitalWrite(GREEN_LED, HIGH);
}  
  

void loop() {
  char  topic[STRING_BUFFER];
  char  message[STRING_BUFFER];

  uip.poll();
  
  if (rf12_recvDone()) {
    if (rf12_crc == 0) {
      count_pkts++;
      
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
      
      // quick flash on receipt of packet
      digitalWrite(GREEN_LED, LOW);
      delay(50);              
      digitalWrite(GREEN_LED, HIGH);  

      Serial.print(topic);
      Serial.print(", ");
      Serial.println(message);
 
  
      
        
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
    } else {
      count_crc++;
    }
  }
}

