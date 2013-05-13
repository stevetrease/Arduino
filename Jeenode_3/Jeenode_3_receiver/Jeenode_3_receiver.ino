#include <JeeLib.h>

// RF stuff
#define RF_NODEID 1        // master node
#define RF_CHANNEL 125

#define BLUE_LED 9

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


 
void setup() {
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);        //off  

  Serial.begin(57600);
  Serial.println("Initialising...");
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
      count_pkts++;
      
      Payload temp = *(Payload*) rf12_data;
      data = temp;
      
      Serial.print("n:");
      Serial.print(data.nodeid);
      
      Serial.print("\tt:");
      Serial.print(data.type);
      
      Serial.print("\td:");
      Serial.print(data.data);     

      
      Serial.print("\t");
      Serial.print(count_crc);
      Serial.print("/");
      Serial.print(count_pkts);
      
      Serial.println(); 
      
      // quick flash on receipt of packet
      digitalWrite(BLUE_LED, LOW);
      delay(50);              
      digitalWrite(BLUE_LED, HIGH);    
      
    } else {
      count_crc++;
      Serial.print("CRC error\t");
      Serial.print(count_crc);
      Serial.print("/");
      Serial.print(count_pkts);
      
      Serial.println();
      // quick double flash on error packet
      digitalWrite(BLUE_LED, LOW);
      delay(50);              
      digitalWrite(BLUE_LED, HIGH);
      delay(50); 
      digitalWrite(BLUE_LED, LOW);
      delay(50);              
      digitalWrite(BLUE_LED, HIGH);
    }
  }
}

