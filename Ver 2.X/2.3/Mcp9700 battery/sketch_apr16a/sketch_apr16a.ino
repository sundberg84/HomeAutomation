// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_NODE_ID 4
#define SKETCH_NAME "Temp Outside South #4"
#define SKETCH_VERSION "1.0"

#include <SPI.h>
#include <MySensors.h>  

#include <Vcc.h>
#define VCC_MIN 2,7
#define VCC_MAX 3.1
const float VccCorrection = 2.9/2.9;  // Measured Vcc by multimeter divided by reported Vcc
Vcc vcc(VccCorrection);

#define CHILD_ID_TEMP 0
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

uint32_t SLEEP_TIME = 120000;  // sleep time between reads (seconds * 1000 milliseconds)

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Battery Meter", "1.0");
  present(CHILD_ID_TEMP, S_TEMP);
 
}


void setup() {

float volt = vcc.Read_Volts();
#ifdef MY_DEBUG
  Serial.print("Battery Voltage: ");
  Serial.print(volt);
  Serial.println(" V");
#endif
    
}

void loop() {
    float temperature = 0.0;   // stores the calculated temperature
    int sample;                // counts through ADC samples
    float ten_samples = 0.0;   // stores sum of 10 samples

    float volt = vcc.Read_Volts();
  
    // take 10 samples from the MCP9700
    for (sample = 0; sample < 10; sample++) {
        // convert A0 value to temperature
        temperature = ((float)analogRead(A0) * volt / 1024.0) - 0.5;
        temperature = temperature / 0.01;
        // sample every 0.1 seconds
        delay(100);
        // sum of all samples
        ten_samples = ten_samples + temperature;
    }
    // get the average value of 10 temperatures
    temperature = ten_samples / 10.0;
    // send temperature out of serial port
    Serial.print(temperature);
    Serial.println(" deg. C");
    ten_samples = 0.0;

    send(msgTemp.set(temperature, 1));

    sleep(SLEEP_TIME);
}
