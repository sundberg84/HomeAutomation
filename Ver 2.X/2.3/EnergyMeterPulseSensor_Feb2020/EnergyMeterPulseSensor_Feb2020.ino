#define MY_DEBUG

#define MY_RADIO_NRF24
#define MY_NODE_ID 8

#include <SPI.h>
#include <MySensors.h>

#define DIGITAL_INPUT_SENSOR 3  // The digital input you attached your light sensor.  (Only 2 and 3 generates interrupt!)
#define PULSE_FACTOR 1000       // Number of blinks per of your meter. Normally 1000.
#define SLEEP_MODE true        // Watt value can only be reported when sleep mode is false.
#define MAX_WATT 10000          // Max watt value to report. This filters outliers.
#define CHILD_ID 1              // Id of the sensor child

uint32_t SEND_FREQUENCY = 4*60000*15; // Minimum time between send (in milliseconds). We don't want to spam the gateway. (Corr * 60sek * minutes)
double ppwh = ((double)PULSE_FACTOR) / 1000; // Pulses per watt hour
bool pcReceived = false;
volatile uint32_t pulseCount = 0;
volatile uint32_t lastBlinkmicros = 0;
volatile uint32_t lastBlinkmillis = 0;
volatile uint32_t watt = 0;
uint32_t oldPulseCount = 0;
uint32_t oldWatt = 0;
double oldkWh;
uint32_t lastSend;
MyMessage wattMsg(CHILD_ID, V_WATT);
MyMessage kWhMsg(CHILD_ID, V_KWH);
MyMessage pcMsg(CHILD_ID, V_VAR1);

#define SKETCH_NAME "Energy Meter #8"                // Change to a fancy name you like
#define SKETCH_VERSION "1.4"                    // Your version

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
int recrequestcount = 0;

void IRQ_HANDLER_ATTR onPulse() {
  if (!SLEEP_MODE) {
    uint32_t newBlinkmicros = micros();
    uint32_t newBlinkmillis = millis();
    uint32_t intervalmicros = newBlinkmicros - lastBlinkmicros;
    uint32_t intervalmillis = newBlinkmillis - lastBlinkmillis;
    if (intervalmicros < 10000L && intervalmillis < 10L) { // Sometimes we get interrupt on RISING
      return;
    }
    if (intervalmillis < 360000) { // Less than an hour since last pulse, use microseconds
      watt = (3600000000.0 / intervalmicros) / ppwh;
    } else {
      watt = (3600000.0 / intervalmillis) /
             ppwh; // more thAn an hour since last pulse, use milliseconds as micros will overflow after 70min
    }
    lastBlinkmicros = newBlinkmicros;
    lastBlinkmillis = newBlinkmillis;
  }
  pulseCount++;
}

void presentation() {

  // Send the Sketch Version Information to the Gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  present(CHILD_ID, S_POWER);
}

void setup()
{

  //=========================
  // BATTERY MEASURER
  //Set internal ref to internal to be able to measure bat 0-1v
  //Make sure this fits other sensors using analogRead()!
  //If you have a sensor reporting 0-5v you need to change analogReference() before reading that sensor.

  analogReference(INTERNAL);
  //DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards) or 3.3 volts (on 3.3V Arduino boards)
  //INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328 and 2.56 volts on the ATmega8 (not available on the Arduino Mega)
  //EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used as the reference.

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

  delay(500); // Allow time for radio if power used as reset

  request(CHILD_ID, V_VAR1);
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), onPulse, RISING);
  lastSend = millis();
}

void loop()
{
  uint32_t now = millis();
  // Only send values at a maximum frequency or woken up from sleep
  bool sendTime = now - lastSend > SEND_FREQUENCY;

  if (pcReceived && (SLEEP_MODE || sendTime)) {
    // New watt value has been calculated
    if (!SLEEP_MODE && watt != oldWatt) {
      // Check that we don't get unreasonable large watt value, which
      // could happen when long wraps or false interrupt triggered
      if (watt < ((uint32_t)MAX_WATT)) {
        send(wattMsg.set(watt));  // Send watt value to gw
      }
      Serial.print("Watt:");
      Serial.println(watt);
      oldWatt = watt;
    }

    // Pulse count value has changed
    if (pulseCount != oldPulseCount) {
      send(pcMsg.set(pulseCount));  // Send pulse count value to gw
      double kWh = ((double)pulseCount / ((double)PULSE_FACTOR));
      oldPulseCount = pulseCount;
      if (kWh != oldkWh) {
        send(kWhMsg.set(kWh, 4));  // Send kWh value to gw
        oldkWh = kWh;
      }
    }
    lastSend = now;
  } else if (now - lastSend > 5000 && !pcReceived) {
    // No pulse count value received from controller. Try requesting it again.
    request(CHILD_ID, V_VAR1);
    recrequestcount = recrequestcount + 1;
#ifdef MY_DEBUG
    Serial.println("Req p-cnt");
#endif
    lastSend = now;
    return;
  }

  else if (recrequestcount == 5 && !pcReceived) {
    //For some controllers, if you dont have any V_VAR1 stored node will not get an answer.
    //Try 5 times, then set V_VAR1 to 0 and update controller
#ifdef MY_DEBUG
    Serial.println("Set p-cnt and update controller");
#endif
    pcReceived = true;
    recrequestcount = 0;
    send(pcMsg.set(pulseCount));  // Send pulse count 0 value to gw
    double kWh = ((double)pulseCount / ((double)PULSE_FACTOR));
    send(kWhMsg.set(kWh, 4));  // Send kWh value 0 to gw
    lastSend = now;
  }

  else if (!pcReceived) {
    //We dont want to sleep if !pcRecieved. Return and wait 5 sec to request a new value.
    return;
  }

  if (SLEEP_MODE) {
    sleep(SEND_FREQUENCY, false);
  }
}


void receive(const MyMessage &message)
{
  if (message.getType() == V_VAR1) {
    pulseCount = oldPulseCount = message.getLong();
    Serial.print("Received last pulse count value from gw:");
    Serial.println(pulseCount);
    pcReceived = true;
  }
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
    batteryPcnt = (batArray[0] + batArray[1] + batArray[2]);
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
