// Enable debug prints
// #define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Set baud rate to same as optibot
#define MY_BAUD_RATE 9600

#define MY_NODE_ID 16
#define SKETCH_NAME "Dec Köket #16"
#define SKETCH_VERSION "1.0"

#include <MySensors.h>
#include <Vcc.h>
#include <SPI.h>

#define CHILD_ID 1   // Id of the sensor child
#define PIR_PIN 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)

int BATTERY_REPORT_BY_IRT_CYCLE = 10;  // Make a battery report after this many trips. Maximum report interval will also be equal to this number of days.
#define ONE_DAY_SLEEP_TIME 86400000
bool interruptReturn = false; // "false" will make the first loop disregard high output from HV-505 (from start-up) and make a battery report instead.  
#define VCC_MIN 1.9
#define VCC_MAX 3.3

// Initialize motion message
MyMessage msg(CHILD_ID, V_TRIPPED);

Vcc vcc;

void setup()  
{  
  pinMode(PIR_PIN, INPUT);      // sets the motion sensor digital pin as input
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MOTION);}

void loop()     
{

      #ifdef MY_DEBUG
       Serial.print("Wakeupcall: "); Serial.println(interruptReturn);
      #endif

    if (interruptReturn == true) {    // Woke up by changing pin
    send(msg.set("1"));
    BATTERY_REPORT_BY_IRT_CYCLE++;
        
          if (BATTERY_REPORT_BY_IRT_CYCLE>9){
          BATTERY_REPORT_BY_IRT_CYCLE=0;
          sendBatteryReport();
          }
    }

  if (interruptReturn == false) {
      sendBatteryReport();
    }

#ifdef MY_DEBUG
  Serial.println("5 sec");
#endif
 
  sleep(10000);  // Make sure everything is stable before start to sleep with interrupts. (don't use "wait()" here). Tests shows false trip ~2s after battery report otherwise.
  send(msg.set("0"));
  sleep(500);  // Make sure everything is stable before start to sleep with interrupts. (don't use "wait()" here). Tests shows false trip ~2s after battery report otherwise.

 // Sleep until interrupt comes in on motion sensor or sleep time passed.
  interruptReturn = sleep(digitalPinToInterrupt(PIR_PIN), RISING, ONE_DAY_SLEEP_TIME );
  
}

void sendBatteryReport() {
  float p = vcc.Read_Perc(VCC_MIN, VCC_MAX, true);
  int batteryPcnt = static_cast<int>(p);
#ifdef MY_DEBUG
  Serial.print("Battery is: "); Serial.println(batteryPcnt);
#endif
  sendBatteryLevel(batteryPcnt);
}
