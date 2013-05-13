// requires the NanodeMAC library from https://github.com/thiseldo/NanodeMAC
// requires the EtherCard library from http://jeelabs.net/projects/cafe/wiki/EtherCard

#include <EtherCard.h>
#include <JeeLib.h>


#define RF_NODEID       1      // master node id
#define RF_CHANNEL      125    // channel ID - needs to be the same as the whole sensor network
#define NUMBER_OF_NODES 15     // zero plus A-Z for sensor nodes

#define BLINK_SPEED     50     // how long to blink LEDs for
#define STATUS_LED      5      // green on nanode
#define ERROR_LED       6      // red on nanode


// Payload data structure
typedef struct {		
      	  int            nodeid;		// sending node ID
          unsigned  long uptime;                // time since node boot      
          long           battery;               // Jeenode battery level       
	  float          data_1;		
          // float          data_2;
          // float          data_3;
          // float          data_4;
} Payload;
Payload data_array[NUMBER_OF_NODES];


static byte mymac[] = { 0x00,0x69,0x69,0x2D,0x30,0x31 };
static BufferFiller bfill;                 // used as cursor while filling the buffer

byte Ethernet::buffer[1000];               // tcp/ip send and receive buffe

long packets_received = 0;                 // counter for RF12 packets received
long packets_CRC = 0;                      // counter for CRCs received


void setup(){ 
  // LED setup and off
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);        
  pinMode(ERROR_LED, OUTPUT);
  digitalWrite(ERROR_LED, HIGH);       

  Serial.begin(57600);
  Serial.println("Initialising...");
  
  // flash both LEDs to signify booting
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(ERROR_LED, LOW);
  delay(BLINK_SPEED);              
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(ERROR_LED, HIGH);
   
  // initialise the radio 
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL); 
    
  // get the Nanode's ethernet address  
  // NanodeMAC mac(mymac);  
    
  // initialise ethernet and get an IP address  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup()) {
    Serial.println("DHCP failed");
    digitalWrite(ERROR_LED, LOW);
    delay(BLINK_SPEED);              
    digitalWrite(ERROR_LED, HIGH);
    delay(BLINK_SPEED); 
    digitalWrite(ERROR_LED, LOW);
    delay(BLINK_SPEED);              
    digitalWrite(ERROR_LED, HIGH);
  }

  ether.printIp("IP:  ", ether.myip);
  
  // flash the LED on OK DHCP 
  digitalWrite(STATUS_LED, LOW);
  delay(BLINK_SPEED);              
  digitalWrite(STATUS_LED, HIGH);
  delay(BLINK_SPEED); 
  digitalWrite(STATUS_LED, LOW);
  delay(BLINK_SPEED);              
  digitalWrite(STATUS_LED, HIGH);
  
  // zero the uptime field. That way we know that any array element with a zero uptime hasn't received a data packet
  for (int i = 0; i < NUMBER_OF_NODES; ++i) {
    data_array[i].uptime = 0;   
  } 
  Serial.println("End of initialization");
}



// 
// main loop 
// on each iteration check for ethernet activity and either return an error page or data page
// then check for a new RF packet and stick it into the data array
//
void loop(){
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    // check if valid tcp data is received
    
    if (pos) {
      
        // flicker error LED on web request
        digitalWrite(ERROR_LED, LOW);
        delay(BLINK_SPEED);              
        digitalWrite(ERROR_LED, HIGH);

        bfill = ether.tcpOffset();
        char* data = (char *) Ethernet::buffer + pos;

        // receive buf hasn't been clobbered by reply yet
        if (strncmp("GET / ", data, 6) == 0) {
             Serial.println("Calling /");  
             bfill.emit_p(PSTR(
                "HTTP/1.0 200 UK\r\n"
                "Content-Type: text/html\r\n"
                "\r\n"));       
             bfill.emit_p(PSTR("<meta http-equiv='refresh' content='5'/>"));
             
             bfill.emit_p(PSTR("Packets received: $D<br>"), packets_received);
             bfill.emit_p(PSTR("CRC errors:       $D<br>"), packets_CRC);
             long t = millis() / 1000;
             word h = t / 3600;
             byte m = (t / 60) % 60;
             byte s = t % 60;
             bfill.emit_p(PSTR("Uptime is $D$D:$D$D:$D$D<br>"), h/10, h%10, m/10, m%10, s/10, s%10);
             Serial.println("Finished with /");
        } else {  
            Serial.println("Calling ELSE");  
            bfill.emit_p(PSTR(
                "HTTP/1.0 200 UK\r\n"
                "Content-Type: text/html\r\n"
                "\r\n")); 
             bfill.emit_p(PSTR("<meta http-equiv='refresh' content='5'/>"));
             // for (int i = 0; i < NUMBER_OF_NODES; ++i) {
                //if (data_array[i].uptime == 0) {
                  // no data in this node yet
                  //Serial.print("nothing in ");
                  //Serial.println(i);
                  
                //} else {
                  //Serial.print("data in ");
                  //Serial.println(i);
                  //bfill.emit_p(PSTR("Data in $D<br>"), i);
                //}
                
                // bfill.emit_p(PSTR("n:$D\tu:$D\tb:$D\td1:$D<br>"), 1, 1, 1, 1);
                //data[i].nodeid, data[i].uptime/1000.0, data[i].battery/1000.0, data[i].data_1);
             //} 
             Serial.println("Finished with ELSE");  
        }
        ether.httpServerReply(bfill.position()); // send web page data
    }
    
  if (rf12_recvDone()) {
    if (rf12_crc == 0) {
      packets_received++;           
      Payload temp = *(Payload*) rf12_data;
      data_array[temp.nodeid] = temp;
      Serial.print("Packet ");   
      Serial.print("n:");
      Serial.print(temp.nodeid);
      Serial.print("\tu:");
      Serial.print(temp.uptime/1000.0);
      Serial.print("\tb:");
      Serial.print(temp.battery/1000.0);
      Serial.print("\td1:");
      Serial.print(temp.data_1);
      Serial.println();            
      
      digitalWrite(STATUS_LED, LOW);
      delay(BLINK_SPEED);              
      digitalWrite(STATUS_LED, HIGH);    
    } else {
      packets_CRC++;
      Serial.println("CRC");  
      digitalWrite(ERROR_LED, LOW);
      delay(BLINK_SPEED);              
      digitalWrite(ERROR_LED, HIGH);
    }
  }  
}
