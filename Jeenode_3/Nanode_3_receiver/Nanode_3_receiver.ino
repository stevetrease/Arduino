#include <JeeLib.h>

// RF stuff
#define RF_NODEID 1        // master node
#define RF_CHANNEL 125

#define MAX_RECORDS 27


#define GREEN_LED 5

// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 5000
// receive buffer size
#define BUFFER_SIZE 64

unsigned int count_crc = 0;
unsigned int count_pkts = 0;

// Payload data structure
typedef struct {		
      	  int            nodeid;		// sending node ID
          char           type;            
	  float          data;		
} Payload;
Payload data;

typedef struct {
    float          data;
    unsigned long  timestamp;
} Records;
Records records[MAX_RECORDS];

 
void setup() {
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);        //off  

  Serial.begin(57600);
  Serial.println("Initialising...");
  // flash the LED to signify booting
  digitalWrite(GREEN_LED, LOW);
  delay(100);              
  digitalWrite(GREEN_LED, HIGH);
  delay(150); 
  digitalWrite(GREEN_LED, LOW);
  delay(200);              
  digitalWrite(GREEN_LED, HIGH);
  
  for (int i = 0; i < MAX_RECORDS; i++) {
      records[i].data =-1000;
  }
}  
  

void loop() {

  
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
     
      records[data.nodeid].data = data.data;
      records[data.nodeid].timestamp = millis();
      
      // quick flash on receipt of packet
      digitalWrite(GREEN_LED, LOW);
      delay(50);              
      digitalWrite(GREEN_LED, HIGH);    
      
      for (int i = 0; i < MAX_RECORDS; i++) {
        if (records[i].data != -1000) {
          Serial.print(i);
          Serial.print(":");
          Serial.print(records[i].data);
          Serial.print(":");
          Serial.print((millis()-records[i].timestamp)/1000);
          Serial.print("\t");
        }
      }       
      Serial.println();
      Serial.println();
    } else {
      count_crc++;
    }
  }
}

