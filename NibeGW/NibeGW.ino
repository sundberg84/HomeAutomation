/**
* This ARDUINO application listening data from Nibe F1145/F1245 heat pumps (RS485 bus - MODBUS bus)
* and sends valid data to a Mysensors (v. >2.0) network . It also sends ACK/NAK to the heat pump so it doesnt go into alarm mode.
* This is an Arduino-based solution, tested with Arduino + RS485 adapter.
*
* When Modbus adapter support is enabled from the heat pump UI, the heat pump will start to send telegrams every now and then.
* According to Modbus datasheet the heatpump software must be greater then version 1161 (? Not verified!)
* A telegram contains a maximum of 20 registers. Those 20 registers can be configured via the Nibe ModbusManager application. 
*
* The sketch uses SoftwareSerial (https://www.arduino.cc/en/Reference/softwareSerial) to read 1 serial and send debug over the other.
* SoftwareSerial uses the same pins as the Nrf24l01+ radio so these needs to be redefined (see defines)!
*
* Wire the radio as usual https://www.mysensors.org/build/connect_radio
* Define which sensors that should be sent through the network in presentation{} and also in void sendMessage()
* Connect the serial hardware to pin 9 - 11 on AA3-X4 pin in the nibe heatpump (see MODBUS installation manual for pictures)
* (9 = GND, 10 = B, 11 = A). 
* DI (data in) on RS485 adapter to pin 8 on the arduino.
* RO (receive out) on RS485 adapter to pin 7 on the arduino.
* DE (data enable) and RE (receive enable) jumpered together on RS485 adapter and to pin 3 on the arduino.
*
*-----------------------------------Notes----------------------------------
*Kör med RealTerm. http://realterm.sourceforge.net/
*Knappar in Hex[space] på Display-tabben. Half duplex.
*9600 BAUD, 8 bits data, 1 bits stop och RS485-rts på Hard Ware Flow Control under Port-tabben.
*Under Send-tabben väljer man +crc under EOL och sedan MOdbus16 från dropdown-menyn.

*Sedan matar jag in 0x01 0x03 0x9C 0x44 0x00 0x01 i send-rutan.
*[0x01] till master, [0x03] Read holding register dvs läs, [0x9C][0x44] är 40004 som betyder läs utomhustemperaturen och så [0x00][0x01] vilket jag antar är hur långt svar jag vill ha(???), hur många register jag vill läsa vilket alltid är 16-bitars register(???). Eller så beror det på vilket typ av register det är.
*-----------------------------------Notes----------------------------------
*
* Author: sundberg84 @ www.mysensors.org, openhardware.io
* This is a fork and development from NibeGW in openhab.
* Original Author: pauli.anttila@gmail.com
* https://github.com/openhab/openhab1-addons/tree/master/bundles/binding/org.openhab.binding.nibeheatpump
*
*-----------------------------------------------------------------------------------------------
* Below follows the original notes from the original author:
* Note that this modified code only uses MySensors network and does not send using ethernet/udp!
*-----------------------------------------------------------------------------------------------
*
* openHAB, the open Home Automation Bus.
* Copyright (c) 2010-2015, openHAB.org <admin@openhab.org>
*
* See the contributors.txt file in the distribution for a
* full listing of individual contributors.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses>.
*
* Additional permission under GNU GPL version 3 section 7
*
* If you modify this Program, or any covered work, by linking or
* combining it with Eclipse (or a modified version of that library),
* containing parts covered by the terms of the Eclipse Public License
* (EPL), the licensors of this Program grant you additional permission
* to convey the resulting work.
*
* ----------------------------------------------------------------------------
*
* This ARDUINO application listening data from Nibe F1145/F1245 heat pumps (RS485 bus)
* and send valid frames to configurable IP/port address by UDP packets.
* Application also acknowledge the valid packets to heat pump.
*
* Ethernet and RS-485 arduino shields are required
*
* Serial settings: 9600 baud, 8 bits, Parity: none, Stop bits 1
*
* MODBUS module support should be turned ON from the heat pump.
*
* Frame format:
* +----+----+----+-----+-----+----+----+-----+
* | 5C | 00 | 20 | CMD | LEN | DATA | CHK |
* +----+----+----+-----+-----+----+----+-----+
*
* Checksum: XOR
*
* When valid data is received (checksum ok),
* ACK (0x06) should be sent to the heat pump.
* When checksum mismatch,
* NAK (0x15) should be sent to the heat pump.
*
* If heat pump does not receive acknowledge in certain time period,
* pump will raise an alarm and alarm mode is activated.
* Actions on alarm mode can be configured. The different alternatives
* are that the Heat pump stops producing hot water (default setting)
* and/or reduces the room temperature.
*
* Author: pauli.anttila@gmail.com
*
*
* 2.11.2013 v1.00 Initial version.
* 3.11.2013 v1.01 
*/

#define MY_DEBUG

#define MY_RADIO_NRF24

#include <MySensors.h>
#include <SPI.h> 
#include <avr/wdt.h>
#include <SoftwareSerial.h>

#define SketchName "MySensors Nibe Heatpump Sensor"
#define SketchVer "1.0"

// direction change pin for RS-485 port
#define directionPin     3
#define SSerialRX        7  //Serial Receive pin
#define SSerialTX        8  //Serial Transmit pin

//Define debug options
#ifdef MY_DEBUG
char verbose = 0; //(Possible 0-3)
char debug_buf[100];
#endif

// state machine states
enum e_state {
STATE_WAIT_START,
STATE_WAIT_DATA,
STATE_OK_MESSAGE_RECEIVED,
STATE_CRC_FAILURE,
};

e_state state = STATE_WAIT_START;

// message buffer for RS-485 communication 
#define MAX_DATA_LEN 255
byte buffer[MAX_DATA_LEN];
byte index = 0;

SoftwareSerial PumpSerial(SSerialRX, SSerialTX); // RX, TX

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo(SketchName, SketchVer);

    // Register all sensors to gw (they will be created as child devices)
    // Following is a list of possible sensors from Nibe and suggestions how to send them;
    present(CHILD_ID, S_MOTION);
}

void setup() {

wdt_enable (WDTO_1S);
  
pinMode(directionPin, OUTPUT);
digitalWrite(directionPin, LOW);

PumpSerial.begin(9600, SERIAL_8N1);

#ifdef MY_DEBUG
if (verbose) {
debugPrint("\nstarted\n");
}
#endif

 // Send test (0x2B = Read device identification and returns model and softwareversion withing 0,5sec.
 // http://www.nibe.se/nibedocuments/16798/031725-8.pdf p.17
digitalWrite(directionPin, HIGH);
delay(1);
PumpSerial.write(0x2B);
PumpSerial.flush();
delay(1);
digitalWrite(directionPin, LOW); 
}

void loop() {

wdt_reset (); 
 
switch(state) {

case STATE_WAIT_START:
if (PumpSerial.available() > 0) {
byte b = PumpSerial.read();

 #ifdef MY_DEBUG
if (verbose > 2) {
sprintf(debug_buf, "%02x", b);
debugPrint(debug_buf);
}
#endif

if (b == 0x5C) {
buffer[0] = b;
index = 1;
state = STATE_WAIT_DATA;

#ifdef MY_DEBUG
if (verbose > 1) {
debugPrint("Frame start found\n");
}
#endif

}
}
break;

case STATE_WAIT_DATA:

if (PumpSerial.available() > 0) {
byte b = PumpSerial.read();

 #ifdef MY_DEBUG
if (verbose > 2) {
sprintf(debug_buf, "%02x", b);
debugPrint(debug_buf);
}
#endif

if (index >= MAX_DATA_LEN)
{
// too long message
state = STATE_WAIT_START;
}
 else
{
buffer[index++] = b;
int msglen = checkNibeMessage(buffer, index);

  # ifdef MY_DEBUG
if (verbose > 1) {
sprintf(debug_buf, "\ncheckMsg=%d\n", msglen);
debugPrint(debug_buf);
}
#endif

switch (msglen) {

case 0: break; // Ok, but not ready
case -1: state = STATE_WAIT_START; break; // Invalid message
case -2: state = STATE_CRC_FAILURE; break; // Checksum error
default: state = STATE_OK_MESSAGE_RECEIVED; break;
}
}
}

break;

case STATE_CRC_FAILURE:

#ifdef MY_DEBUG
if (verbose) {
debugPrint("CRC failure\n");
}
#endif

sendNak();
state = STATE_WAIT_START;

break;

case STATE_OK_MESSAGE_RECEIVED:

#ifdef MY_DEBUG
if (verbose) {
debugPrint("Message received\n");
}
#endif

sendAck();
  
// send UDP packet if message is a data packet
// if data contains 0x5C (start character), message len can be bigger than 0x50 

if (buffer[0] == 0x5C && buffer[1] == 0x00 && buffer[2] == 0x20 && buffer[3] == 0x68 && buffer[4] >= 0x50)

sendMessage(buffer, index);
state = STATE_WAIT_START;

break;
}
}

// ######### FUNCTIONS #######################
/*
 * Return:
 *  >0 if valid message received (return message len)
 *   0 if ok, but message not ready
 *  -1 if invalid message
 *  -2 if checksum fails
 */
int checkNibeMessage(const byte* const data, byte len)
{
    #ifdef ENABLE_DEBUG
    if (verbose > 2) {
      sprintf(debug_buf, "\nlen=%u\n", len);
      debugPrint(debug_buf);
    }
    #endif
    
    if (len <= 0)
        return 0;
    
    if (len >= 1)
    {
        if (data[0] != 0x5C)
            return -1;
        
        if (len >= 3)
        {
            if (data[1] != 0x00 && data[2] != 0x20)
                return -1;
        }
        
        if (len >= 6)
        {
            int datalen = data[4];
            
            if (len < datalen + 6)
                return 0;
            
            byte checksum = 0;
            
            // calculate XOR checksum
            for(int i = 2; i < (datalen + 5); i++)
                checksum ^= data[i];
            
            byte msg_checksum = data[datalen + 5];
            
            #ifdef ENABLE_DEBUG
            if (verbose > 1) {
              sprintf(debug_buf, "\nchecksum=%02x, msg_checksum=%02x\n", checksum, msg_checksum);
              debugPrint(debug_buf);
            }
            #endif
                     
            if (checksum != msg_checksum) {
              
              // check special case, if checksum is 0x5C (start character), 
              // heat pump seems to send 0xC5 checksum
              if (checksum != 0x5C && msg_checksum != 0xC5)
                return -2;
            }  
            
            return datalen + 6;
        }   
    }
    return 0;   
}

void sendAck()
{

#ifdef MY_DEBUG
if (verbose) {
debugPrint("Send ACK\n");
}
#endif

digitalWrite(directionPin, HIGH);
delay(1);
PumpSerial.write(0x06);
PumpSerial.flush();
delay(1);
digitalWrite(directionPin, LOW);

}

void sendNak()
{

#ifdef MY_DEBUG
if (verbose) {
debugPrint("Send NAK\n");
}
#endif

digitalWrite(directionPin, HIGH);
delay(1);
PumpSerial.write(0x15);
PumpSerial.flush();
delay(1);
digitalWrite(directionPin, LOW);
}

void sendMessage(const byte* const data, int len) {

#ifdef MY_DEBUG
if (verbose) {
sprintf(debug_buf, "Sending MySensors Message, len=%d\n", len);
debugPrint(debug_buf);
if (verbose > 2) {
int i;
for (i=0; i<len; i++) {
sprintf(debug_buf, "%02x", data[i]);
debugPrint(debug_buf);
}
 debugPrint("\n");
}
}
#endif

//OUTPUT HÄR (msg.send)

}

#ifdef MY_DEBUG
void debugPrint(char* data) {

 Serial.println(data);

}
#endif
