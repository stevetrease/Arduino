#include <JeeLib.h>

Port one(1), two(2);

#define BLUE_LED 9

#define VOLTS 3.3



// this must be added since we're using the watchdog for low-power waiting
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 10000

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

  one.mode2(INPUT);
  two.mode2(INPUT);

  //Serial.begin(57600);
  //Serial.println("Initialising...");
  // flash the LED to signify booting
  digitalWrite(BLUE_LED, LOW);
  delay(100);              
  digitalWrite(BLUE_LED, HIGH);
  delay(150); 
  digitalWrite(BLUE_LED, LOW);
  delay(200);              
  digitalWrite(BLUE_LED, HIGH);
}  
  

float tempC, tempX, tempY;

void loop() { 
  
  tempC = one.anaRead();           
  tempX = (VOLTS * tempC * 100.0)/1024.0; 
  data.type = 'X';
  data.data = tempX;
  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
  rf12_sendWait(5);
  delay(5000);

  tempC = one.anaRead();           
  tempY = (VOLTS * tempC * 100.0)/1024.0; 
  data.type = 'Y';
  data.data = (float) tempY;
  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
  rf12_sendWait(5);
  delay(5000);
  
  data.type = 'V';
  data.data = readVcc()/1000;
  while (!rf12_canSend()) rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
  rf12_sendWait(5);
  delay(5000);

  
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
