#include <JeeLib.h>

// RF stuff
#define RF_NODEID 1
#define RF_CHANNEL 111

#define BLUE_LED 9

// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 5000
// receive buffer size
#define BUFFER_SIZE 64


// Payload data structure
typedef struct {		
      	  int            nodeid;		// sending node ID
          unsigned  long uptime;                // time since node boot      
          float          battery;               // Jeenode battery level       
	  float          data_1;		
          float          data_2;
          float          data_3;
          float          data_4;
} Payload;
Payload data;


 
void setup() {
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);        //off  

  Serial.begin(57600);
  Serial.println("Initalising...");
  // flash the LED to signify booting
  digitalWrite(BLUE_LED, LOW);
  delay(100);              
  digitalWrite(BLUE_LED, HIGH);
  delay(150); 
  digitalWrite(BLUE_LED, LOW);
  delay(200);              
  digitalWrite(BLUE_LED, HIGH);
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
      Serial.print(data.nodeid);
      Serial.print("\t");
      Serial.print(data.uptime/1000.0);
      Serial.print("\t");
      Serial.print(data.data_1);
      Serial.println();      
      
      // quick double flash on receipt of packet
      digitalWrite(BLUE_LED, LOW);
      delay(75);              
      digitalWrite(BLUE_LED, HIGH);
      delay(75); 
      digitalWrite(BLUE_LED, LOW);
      delay(75);              
      digitalWrite(BLUE_LED, HIGH);      
    } 
  }
}

