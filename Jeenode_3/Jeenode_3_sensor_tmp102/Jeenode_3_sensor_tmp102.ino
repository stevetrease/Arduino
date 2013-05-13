#include <JeeLib.h>
#include <Ports.h>


//    TMP102 on Jeenode port 1
//      SDA to DIO
//      SCL to AIO
int tmp102Address = 0x48;  // ADD0 to ground on TMP102
PortI2C   myport(1, PortI2C::KHZ100);
DeviceI2C temperature(myport, tmp102Address); 

// RF stuff
#define BLUE_LED 9

// this must be added since we're using the watchdog for low-power waiting
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 30000

//data Structure to be sent, called payload

typedef struct {		
      	  int            nodeid;		// sending node ID
          char           type;
          float          data;		
} Payload;
Payload data;

 
void setup() {
  // use channel,noce and frequency from EPROM
  data.nodeid = rf12_config(); 

  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);        //off  

  // Serial.begin(57600);
  // Serial.println("Initalising...");
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
    
  data.type = 'A';
  data.data = getTemperature();
  while (!rf12_canSend()) rf12_recvDone();
  rf12_sendStart(0, &data, sizeof data);
  rf12_sendWait(2);
  delay(50);

 
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


// Function to get temperature from TMP102 and return it as a float in Celsius
float getTemperature()
{   
  temperature.send();             // go into send mode
  temperature.write(0x00);        //
  temperature.receive();          // grab it
  byte MSB = temperature.read(0); // Whole degrees
  byte LSB = temperature.read(1); // fractional degrees
  temperature.stop();             // done

  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
  float celsius = TemperatureSum*0.0625;  
  
  // Serial.print(MSB,BIN);
  // Serial.print(" ");
  // Serial.print(LSB,BIN);
  // Serial.print("\t");
  // Serial.print(TemperatureSum, BIN);
  // Serial.print("\t");
  // Serial.print(celsius);
  // Serial.println();
  
  return celsius;
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
