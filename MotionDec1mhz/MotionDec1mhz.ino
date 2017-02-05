// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Set baud rate to same as optibot
#define MY_BAUD_RATE 9600

#define MY_NODE_ID 10
#define SKETCH_NAME "Taklampa Hallen #10"
#define SKETCH_VERSION "1.1"

#include <MySensors.h>
#include <Vcc.h>
#include <SPI.h>

unsigned long SLEEP_TIME = 900000UL; // Sleep time between reports (in milliseconds)
#define PIR_PIN 3
#define BATTERY_REPORT_DAY 2   // Desired heartbeat(battery report) interval when inactive. 
#define BATTERY_REPORT_BY_IRT_CYCLE 10  // Make a battery report after this many trips. Maximum report interval will also be equal to this number of days.
#define ONE_DAY_SLEEP_TIME 86400000
#define VCC_MIN 1.9
#define VCC_MAX 3.3
#define CHILD_ID 0
int dayCounter = BATTERY_REPORT_DAY;
int irtCounter = 0;
bool interruptReturn = false; // "false" will make the first loop disregard high output from HV-505 (from start-up) and make a battery report instead.
int oldTripped = 0;
int tripped = 0;


Vcc vcc;
MyMessage msg(CHILD_ID, V_TRIPPED);

void setup()
{
#ifdef MY_DEBUG
  Serial.println("Serial started");
#endif
  delay(100); // to settle power for radio
  pinMode(PIR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT_PULLUP);
#ifdef MY_DEBUG
  Serial.println("Warming and blocking PIR trip for 20s.");
#endif
  sleep(20000); // Wait until HC-505 warmed-up and output returned low.
}

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MOTION);
}

void loop()
{
  if (interruptReturn) {    // Woke up by changing pin

#ifdef MY_DEBUG
    Serial.println("Returned from sleep because of interupt and will check motion: ");
#endif

    tripped = digitalRead(PIR_PIN);
    if (tripped != oldTripped) {
#ifdef MY_DEBUG
      Serial.println("Tripped changed - sending new state!");
#endif
      send(msg.set(tripped ? "1" : "0")); // Send tripped value to gw
      oldTripped = tripped;
    }
  }

  irtCounter++;
  if (irtCounter >= BATTERY_REPORT_BY_IRT_CYCLE) {
    irtCounter = 0;
    sendBatteryReport();
  }

  else { // Woke up by timer  (or it's the first run)
    dayCounter++;
    if (dayCounter >= BATTERY_REPORT_DAY) {
      dayCounter = 0;
      sendBatteryReport();
    }
  }

#ifdef MY_DEBUG
  Serial.println("3 sec sleep to avoid false trips");
#endif
sleep(3000);  // Make sure everything is stable before start to sleep with interrupts. (don't use "wait()" here). Tests shows false trip ~2s after battery report otherwise.

// Sleep until interrupt comes in on motion sensor or sleep time passed.
interruptReturn = sleep(digitalPinToInterrupt(PIR_PIN), RISING, SLEEP_TIME);

}

void sendBatteryReport() {
  float p = vcc.Read_Perc(VCC_MIN, VCC_MAX, true);
  int batteryPcnt = static_cast<int>(p);
#ifdef MY_DEBUG
  Serial.print("Battery is: "); Serial.println(batteryPcnt);
#endif
  sendBatteryLevel(batteryPcnt);
}

