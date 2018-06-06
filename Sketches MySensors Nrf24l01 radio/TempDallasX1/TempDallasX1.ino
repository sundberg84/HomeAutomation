// Enable debug prints
//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

//Fixed ID/Parent?
#define MY_NODE_ID 9                     //To set a fixed ID for your node
//#define MY_PARENT_NODE_ID 100             //To set a fixed parent for this node

#include <SPI.h>
#include <MySensors.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

#define CHILD_ID_TEMP 0
unsigned long SLEEP_TIME = 600000; // Sleep time between reads (in milliseconds)

#define SKETCH_NAME "Gillestuga #9"        // Change to a fancy name you like
#define SKETCH_VERSION "1.3"                    // Your version

boolean metric = true;
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);


//=========================
// BATTERY MEASURER
// VOLTAGE DIVIDER SETUP
// 1M, 470K divider across battery and using internal ADC ref of 1.1V
// Sense point is bypassed with 0.1 uF cap to reduce noise at that point
// ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
// 3.44/1023 = Volts per bit = 0.003363075
#define VBAT_PER_BITS 0.003363075
#define VMIN 2.2                                  //  Vmin (radio Min Volt)=1.9V (564v)
#define VMAX 3.1                                  //  Vmax = (2xAA bat)=3.0V (892v)
int batteryPcnt = 0;                              // Calc value for battery %
int batLoop = 0;                                  // Loop to help calc average
int batArray[3];                                  // Array to store value for average calc.
int BATTERY_SENSE_PIN = A0;                       // select the input pin for the battery sense point
//=========================

void setup()
{
  analogReference(INTERNAL);             // For battery sensing

  delay(500); // Allow time for radio if power used as reset

  // start serial port
  Serial.println("Dallas Temperature IC Control Library");

  // locate devices on the bus
  Serial.println("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  //Battery inital calc
  Serial.print("With Battery VMax (100%) = "); Serial.print(VMAX); Serial.print("volts and Vmin (0%) = "); Serial.print(VMIN); Serial.println(" volts");
  Serial.print("Battery Percent 25%/50%/75% calculates to: "); Serial.print(((VMAX - VMIN) / 4) + VMIN); Serial.print("/"); Serial.print(((VMAX - VMIN) / 2) + VMIN); Serial.print("/"); Serial.println(VMAX - ((VMAX - VMIN) / 4));
  delay(1000);
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(50);
  float Vbat  = sensorValue * VBAT_PER_BITS;
  int batteryPcnt = static_cast<int>(((Vbat - VMIN) / (VMAX - VMIN)) * 100.);
  Serial.print("Current battery are measured to (please confirm!): "); Serial.print(batteryPcnt); Serial.print(" % - Or "); Serial.print(Vbat); Serial.println(" Volts");
  //=========================


}

void presentation()
{
  // Send the Sketch Version Information to the Gateway
  // Send the Sketch Version Information to the Gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_TEMP, S_TEMP);
}

void loop()
{
  delay(500); // Allow time for radio if power used as reset



  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
#ifdef MY_DEBUG
  Serial.print("Requesting temperatures...");
#endif
  sensors.requestTemperatures(); // Send the command to get temperatures
#ifdef MY_DEBUG
  Serial.println("DONE");
#endif

send(msgTemp.set(sensors.getTempCByIndex(0), 1));

  MeasureBattery();
  sleep(SLEEP_TIME); //sleep a bit
}

//=========================
// BATTERY MEASURER
void MeasureBattery() //The battery calculations
{
  delay(500);
  // Battery monitoring reading
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(500);

  // Calculate the battery in %
  float Vbat  = sensorValue * VBAT_PER_BITS;
  int batteryPcnt = static_cast<int>(((Vbat - VMIN) / (VMAX - VMIN)) * 100.);
#ifdef MY_DEBUG
  Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.print(" %"); Serial.print("Battery Voltage: "); Serial.print(Vbat); Serial.println(" Volts");
#endif

  // Add it to array so we get an average of 3 (3x20min)
  batArray[batLoop] = batteryPcnt;

  if (batLoop > 2) {
    batteryPcnt = (batArray[0] + batArray[1] + batArray[2] + batArray[3]);
    batteryPcnt = batteryPcnt / 3;

    if (batteryPcnt > 100) {
      batteryPcnt = 100;
    }
#ifdef MY_DEBUG
    Serial.print("Battery Average (Send): "); Serial.print(batteryPcnt); Serial.println(" %");
#endif
    sendBatteryLevel(batteryPcnt);
    batLoop = 0;
  }
  else
  {
    batLoop++;
  }
}
//=========================

