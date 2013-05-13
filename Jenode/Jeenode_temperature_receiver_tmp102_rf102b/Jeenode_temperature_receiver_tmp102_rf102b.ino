#include <JeeLib.h>

// RF stuff
#define RF_NODEID 6
#define RF_CHANNEL 111

// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 5000
// receive buffer size
#define BUFFER_SIZE 64


// Payload data structure
typedef struct {		//data Structure to be sent, called payload
    	  int   node;		// sending node ID
	  float data;		// data
} Payload;
Payload data;


 
void setup() {
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  Serial.begin(57600);
  Serial.println("Initalising...");
}  
  

void loop() {
  if (rf12_recvDone()) {
    if (rf12_crc == 0) {
      Serial.print(millis()/1000.0);
      Serial.print("\t");
      Serial.print(rf12_hdr, BIN);
      Serial.print("\t");
      Serial.print(rf12_len);
      Serial.print("\t");
      
      data=*(Payload*) rf12_data;
      Serial.print(data.node);
      Serial.print("\t");
      Serial.print(data.data);
      Serial.println();      
    } 
  }
}

