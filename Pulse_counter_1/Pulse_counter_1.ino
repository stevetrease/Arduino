#include <EEPROM.h>
#include <JeeLib.h>

#define LED_RED 6
#define LED_GREEN 5
#define CONFIG_START 0


#define RF_NODEID  20       
#define RF_CHANNEL 125


//data Structure to be sent, called payload
typedef struct {		
      	  int            nodeid;		// sending node ID
          char           type;                  // 
          float          data;                // gas
} Payload;
Payload data;



volatile boolean state = LOW;
volatile unsigned long count;
volatile boolean changed = false;

void setup ()
{ 
  unsigned long temp;
  
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  // use channel,node and frequency from EPROM
  data.nodeid = RF_NODEID; 

  
  Serial.begin(57600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
 Serial.print (data.nodeid);
 
  
  // light LED at boot
  digitalWrite(LED_RED, false);
  digitalWrite(LED_GREEN, false);
  
  delay(2000);


  // used to reset pulse counter
  // temp = 0;
  // for (unsigned int t=0; t<sizeof(temp); t++)
     //EEPROM.write(CONFIG_START + t, *((char*)&temp + t));
 
  Serial.println("Load count from EEPROM: ");
  for (unsigned int t=0; t<sizeof(temp); t++)
      *((char*)&temp + t) = EEPROM.read(CONFIG_START + t);
  count = temp;
  
  Serial.print(count);
  Serial.println();

  // and start the interuppt counting
  attachInterrupt(1, pulse, CHANGE); 
}



void loop () 
{
    if (changed == true) {
      Serial.println(count);
      changed = false;
      for (unsigned int t=0; t<sizeof(count); t++)
        EEPROM.write(CONFIG_START + t, *((char*)&count + t));
        
      Serial.println("sent@");
        
      data.type = 'P';
      data.data = (float) count;
      while (!rf12_canSend())
        rf12_recvDone();
      rf12_sendStart(0, &data, sizeof data);
      rf12_sendWait(2);
      Serial.println("sent@");
    }
    digitalWrite(LED_RED, state);
    digitalWrite(LED_GREEN, !state);
}


void pulse ()
{
  count++;
  changed = true;
  state = !state;
}


