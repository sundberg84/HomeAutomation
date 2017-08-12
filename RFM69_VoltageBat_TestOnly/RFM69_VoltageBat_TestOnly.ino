
// Enable debug prints to serial monitor
#define MY_DEBUG 

#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY RFM69_433MHZ
#define MY_NODE_ID 20
#define SKETCH_NAME "Test #20"
#define SKETCH_VERSION "1.0"

#include <SPI.h>
#include <MySensors.h>  
#include <Vcc.h>

#define CHILD_ID 1   // Id of the sensor child
#define VCC_MIN 1.9
#define VCC_MAX 3.3


MyMessage msg(CHILD_ID, V_VOLTAGE);

Vcc vcc;

void setup() {
  // put your setup code here, to run once:

}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MULTIMETER);}


void loop() {
  // put your main code here, to run repeatedly:

     sendBatteryReport();
     float volts = vcc.Read_Volts();
    send(msg.set(volts,1));
 
  sleep(900000);  // Make sure everything is stable before start to sleep with interrupts. (don't use "wait()" here). Tests shows false trip ~2s after battery report otherwise.

}

void sendBatteryReport() {
 
  float p = vcc.Read_Perc(VCC_MIN, VCC_MAX, true);
  int batteryPcnt = static_cast<int>(p);
#ifdef MY_DEBUG
  Serial.print("Battery is: "); Serial.println(batteryPcnt);
#endif
  sendBatteryLevel(batteryPcnt);
}
