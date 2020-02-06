
/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2018 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * DESCRIPTION
 *
 * This is an example that demonstrates how to report the battery level for a sensor
 * Instructions for measuring battery capacity on A0 are available here:
 * http://www.mysensors.org/build/battery
 *
 */



// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24
//#define MY_RADIO_NRF5_ESB
//#define MY_RADIO_RFM69
//#define MY_RADIO_RFM95

#define MY_NODE_ID 4
#define SKETCH_NAME "Batterytester #4"
#define SKETCH_VERSION "1.0"

#include <MySensors.h>

#define CHILD_ID_BATT 1
MyMessage msgBatt(CHILD_ID_BATT, V_VOLTAGE);
#include <Vcc.h>


#define VCC_MIN 2,6
#define VCC_MAX 3.0
const float VccCorrection = 2.9/2.9;  // Measured Vcc by multimeter divided by reported Vcc
Vcc vcc(VccCorrection);
int led = 3;

uint32_t SLEEP_TIME = 120000;  // sleep time between reads (seconds * 1000 milliseconds)

void setup()
{
  pinMode(led, OUTPUT);
}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Battery Meter", "1.0");
 
}

void loop()
{
	// get the battery Voltage


  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  wait(3000);
  float v = vcc.Read_Volts();
  wait(2000);
  float p = vcc.Read_Perc(VCC_MIN, VCC_MAX);
  int batteryPcnt = static_cast<int>(p);
  wait(500);
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

  
#ifdef MY_DEBUG
	Serial.print("Battery Voltage: ");
	Serial.print(v);
	Serial.println(" V");

	Serial.print("Battery percent: ");
	Serial.print(batteryPcnt);
	Serial.println(" %");
#endif

		sendBatteryLevel(batteryPcnt);
    send(msgBatt.set(v,2));

      sleep(SLEEP_TIME);
	}
