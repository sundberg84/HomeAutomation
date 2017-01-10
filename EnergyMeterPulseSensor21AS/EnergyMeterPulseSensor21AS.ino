#define MY_DEBUG 
#define MY_RADIO_NRF24
#define MY_NODE_ID 8

#include <SPI.h>
#include <MySensors.h>  

#define DIGITAL_INPUT_SENSOR 3  // The digital input you attached your light sensor.  (Only 2 and 3 generates interrupt!)
#define PULSE_FACTOR 1000       // Nummber of blinks per KWH of your meeter
#define SLEEP_MODE true        // Watt-value can only be reported when sleep mode is false.
#define MAX_WATT 10000          // Max watt value to report. This filetrs outliers.
#define INTERRUPT DIGITAL_INPUT_SENSOR-2 // Usually the interrupt = pin -2 (on uno/nano anyway)
#define CHILD_ID 1              // Id of the sensor child

double ppwh = ((double)PULSE_FACTOR)/1000; // Pulses per watt hour
boolean pcReceived = false;
volatile unsigned long pulseCount = 0;   
volatile unsigned long lastBlink = 0;
volatile unsigned long watt = 0;
unsigned long oldPulseCount = 0;   
unsigned long oldWatt = 0;
double oldKwh;
unsigned long lastSend;
MyMessage wattMsg(CHILD_ID,V_WATT);
MyMessage kwhMsg(CHILD_ID,V_KWH);
MyMessage pcMsg(CHILD_ID,V_VAR1);

#define SKETCH_NAME "Energy Meter #8"                // Change to a fancy name you like
#define SKETCH_VERSION "1.4"                    // Your version

unsigned long SLEEP_TIME = 780000UL;            // Sleep time (in milliseconds).

//=========================
// BATTERY VOLTAGE DIVIDER SETUP
// 1M, 470K divider across battery and using internal ADC ref of 1.1V
// Sense point is bypassed with 0.1 uF cap to reduce noise at that point
// ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
// 3.44/1023 = Volts per bit = 0.003363075
#define VBAT_PER_BITS 0.003363075  
#define VMIN 1.9                                  //  Vmin (radio Min Volt)=1.9V (564v)
#define VMAX 3.0                                  //  Vmax = (2xAA bat)=3.0V (892v)
int batteryPcnt = 0;                              // Calc value for battery %
int batLoop = 0;                                  // Loop to help calc average
int batArray[3];                                  // Array to store value for average calc.
int BATTERY_SENSE_PIN = A0;                       // select the input pin for the battery sense point
//=========================

void presentation() {

  // Send the Sketch Version Information to the Gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register this device as Rain sensor (will not show in Domoticz until first value arrives)
  present(CHILD_ID, S_RAIN);       
}

void setup()  
{  
  
  analogReference(INTERNAL);             // For battery sensing

  delay(500); // Allow time for radio if power used as reset
  
  // Fetch last known pulse count value from gw
  request(CHILD_ID, V_VAR1);
  
  attachInterrupt(INTERRUPT, onPulse, RISING);
  lastSend=millis();
}


void loop()     
{ 
  unsigned long currentTime = millis();

    //See if we have the counter/pulse from Domoticz - and ask for it if we dont.
    if (!pcReceived && (currentTime - lastSend > 5000)) {      
      request(CHILD_ID, V_VAR1);
      lastSend=currentTime;
      return;
    }
    
    if (!pcReceived) {
      return;
    }
  
  
    // Pulse cout has changed
    if (pulseCount > oldPulseCount + 333) {
      send(pcMsg.set(pulseCount));  // Send pulse count value to gw 
      double kwh = ((double)pulseCount/((double)PULSE_FACTOR));     
      oldPulseCount = pulseCount;
      if (kwh != oldKwh) {
        send(kwhMsg.set(kwh, 4));  // Send kwh value to gw 
        oldKwh = kwh;
        batM();
      }
    }    
    lastSend = currentTime;
  
    
    sleep(SLEEP_TIME);
}

void receive(const MyMessage &message) {
  if (message.type==V_VAR1) {  
    pulseCount = oldPulseCount = message.getLong();
    Serial.print("Received last pulse count from gw:");
    Serial.println(pulseCount);
    pcReceived = true;
  }
}

void onPulse()     
{ 
  pulseCount++;
}

void batM() //The battery calculations
{
   delay(500);
   // Battery monitoring reading
   int sensorValue = analogRead(BATTERY_SENSE_PIN);    
   delay(500);
   
   // Calculate the battery in %
   float Vbat  = sensorValue * VBAT_PER_BITS;
   int batteryPcnt = static_cast<int>(((Vbat-VMIN)/(VMAX-VMIN))*100.);
   Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.println(" %");  
   
   // Add it to array so we get an average of 3 (3x20min)
   batArray[batLoop] = batteryPcnt;
  
   if (batLoop > 2) {  
     batteryPcnt = (batArray[0] + batArray[1] + batArray[2] + batArray[3]);
     batteryPcnt = batteryPcnt / 3;
 
   if (batteryPcnt > 100) {
     batteryPcnt=100;
 }
 
     Serial.print("Battery Average (Send): "); Serial.print(batteryPcnt); Serial.println(" %");
       sendBatteryLevel(batteryPcnt);
       batLoop = 0;
      }
     else 
     {
     batLoop++;
     }
}
