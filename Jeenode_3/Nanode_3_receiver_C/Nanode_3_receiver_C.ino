#include <JeeLib.h>
#include <EtherCard.h>




#include <EtherCard.h>

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x32 };
byte Ethernet::buffer[1000]; // tcp/ip send and receive buffer
static BufferFiller bfill;



// RF stuff
#define RF_NODEID 1        // master node
#define RF_CHANNEL 125


#define GREEN_LED 5   // nanode
#define RED_LED 6     // nanode
#define FLASH_TIME 50 // 50

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

#define MAX_RECORDS 27
typedef struct {
    float          data;
    unsigned long  timestamp;
} Records;
Records records[MAX_RECORDS];

 
 
 
 
void setup() {
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);        //off  

  Serial.begin(57600);
  Serial.println("Initialising...");
  // flash the LED to signify booting
  digitalWrite(GREEN_LED, LOW);
  delay(FLASH_TIME);              
  digitalWrite(GREEN_LED, HIGH);
  delay(FLASH_TIME); 
  digitalWrite(GREEN_LED, LOW);
  delay(FLASH_TIME);              
  digitalWrite(GREEN_LED, HIGH);
  
  ether.begin(sizeof Ethernet::buffer, mymac);
  ether.dhcpSetup();
  
  for (int i = 0; i < MAX_RECORDS; i++) {
      records[i].data =-1000;
  }
}  
  

void loop() {
  
   word len = ether.packetReceive();
   word pos = ether.packetLoop(len);
   // check if valid tcp data is received
   if (pos) {
     bfill = ether.tcpOffset();
     digitalWrite(GREEN_LED, LOW);
     delay(FLASH_TIME);              
     digitalWrite(GREEN_LED, HIGH);    
     char* data = (char *) Ethernet::buffer + pos;
     if (strncmp("GET / ", data, 6) == 0) {
       bfill.emit_p(PSTR(
         "HTTP/1.0 200 OK\r\n"
         "Content-Type: text/html\r\n"
         "Pragma: no-cache\r\n"
         "\r\n"
         "<html>"
         "<head>"
         "<meta http-equiv='refresh' content='15'/>"
         "<title>Jeenode data</title></head><body>")); 

        for (int i = 0; i < MAX_RECORDS; i++) {
          if (records[i].data != -1000) {
            bfill.emit_p(PSTR("<br>$D:$D.$D$D:$D"), i, int(records[i].data),
              int((records[i].data - int(records[i].data))*10),
              int((records[i].data*10 - int(records[i].data*10))*10),
              (millis() - records[i].timestamp)/1000);
          } 
        }  
        bfill.emit_p(PSTR("<P>Good:CRC\t$D:$D"),count_pkts,count_crc);
        long t = millis() / 1000;
        word h = t / 3600;
        byte m = (t / 60) % 60;
        byte s = t % 60;
        bfill.emit_p(PSTR(
          "<br>Uptime is $D$D:$D$D:$D$D"), h/10, h%10, m/10, m%10, s/10, s%10);
        bfill.emit_p(PSTR("</body></html>"));
        ether.httpServerReply(bfill.position()); // send web page data
      // quick flash on receipt of packet
      digitalWrite(GREEN_LED, LOW);
      delay(FLASH_TIME);              
      digitalWrite(GREEN_LED, HIGH);    
      delay(FLASH_TIME);
      digitalWrite(GREEN_LED, LOW);
      delay(FLASH_TIME);              
      digitalWrite(GREEN_LED, HIGH);    
      }
   }
  
  if (rf12_recvDone()) {
    if (rf12_crc == 0) {
      count_pkts++;
      
      Payload temp = *(Payload*) rf12_data;
      data = temp;
      
      //Serial.print("n:");
      //Serial.print(data.nodeid);
      
      //Serial.print("\tt:");
      //Serial.print(data.type);
      
      //Serial.print("\td:");
      //Serial.print(data.data);     
   
      //Serial.println();
     
      if (data.nodeid < MAX_RECORDS) {
        records[data.nodeid].data = data.data;
        records[data.nodeid].timestamp = millis();
      }
      
      
      // quick flash on receipt of packet
      digitalWrite(GREEN_LED, LOW);
      delay(FLASH_TIME);              
      digitalWrite(GREEN_LED, HIGH);    
      
      //for (int i = 0; i < MAX_RECORDS; i++) {
      //  if (records[i].data != -1000) {
      //    Serial.print(i);
      //    Serial.print(":");
      //    Serial.print(records[i].data);
      //    Serial.print(":");
      //    Serial.print((millis()-records[i].timestamp)/1000);
      //    Serial.print("\t");
      //  }
      //}       
      //Serial.println();
      //Serial.println();
    } else {
      count_crc++;
      // quick flash on CRC of packet
      digitalWrite(RED_LED, HIGH);
      delay(FLASH_TIME);              
      digitalWrite(RED_LED, LOW);
    }
  }
}

