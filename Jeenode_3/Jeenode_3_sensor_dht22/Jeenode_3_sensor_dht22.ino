#include <JeeLib.h>


//    DHT222 on port 1 (Arduino 4 == Jeenode P1 DIO1

// RF stuff
#define BLUE_LED 9



#include <dht.h>
dht DHT;
#define DHT22_PIN 4
#define DHT11_PIN 5

// this must be added since we're using the watchdog for low-power waiting
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 30000

//data Structure to be sent, called payload

typedef struct {		
      	  int            nodeid;		// sending node ID
          char           type;                  // 
          float          data;                // humidity
} Payload;
Payload data;

 
void setup() {
  
  // use channel,node and frequency from EPROM
  data.nodeid = rf12_config(); 

  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);        //off  

  // Serial.begin(57600);
  // Serial.println("Initialising...");
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
  
  DHT.read22(DHT22_PIN);
    
  data.type = 'A';
  data.data = (float) DHT.temperature;
  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
  rf12_sendWait(2);
  delay(50);delay(50);
 
  data.type = 'B';
  data.data = (float) DHT.humidity;
  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
  rf12_sendWait(2);
  
  // data.type = 'P';
  // data.data = readVcc()/1000;
  // while (!rf12_canSend()) rf12_recvDone();
  // rf12_sendStart(0, &data, sizeof data);
  // rf12_sendWait(2);
  // delay(50);

   
  // data.type = 'U';
  // data.data = millis();
  // while (!rf12_canSend()) rf12_recvDone();
  // rf12_sendStart(0, &data, sizeof data);
  // rf12_sendWait(2);
  // delay(50);


  
  // quick flash on sending packet
  digitalWrite(BLUE_LED, LOW);
  delay(50);              
  digitalWrite(BLUE_LED, HIGH);
    
  rf12_sleep(RF12_SLEEP);
  Sleepy::loseSomeTime(DELAY);
  rf12_sleep(RF12_WAKEUP);
}




long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}
