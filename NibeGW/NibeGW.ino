/**
 *  This ARDUINO application listening data from Nibe F1145/F1245 heat pumps (RS485 bus)
 *  and send valid frames to Mysensors network.
 *  Application also acknowledge the valid packets to heat pump.
 *
 *  RS-485 shield or module connected to arduino are required.
 *
 *  Serial settings: 9600 baud, 8 bits, Parity: none, Stop bits 1
 *
 *  MODBUS module support should be turned ON from the heat pump.
 *
 *  Frame format:
 *  +----+----+----+-----+-----+----+----+-----+
 *  | 5C | 00 | 20 | CMD | LEN |  DATA   | CHK |
 *  +----+----+----+-----+-----+----+----+-----+
 *
 *  Checksum: XOR
 *
 *  When valid data is received (checksum ok),
 *   ACK (0x06) should be sent to the heat pump.
 *  When checksum mismatch,
 *   NAK (0x15) should be sent to the heat pump.
 *
 *  If heat pump does not receive acknowledge in certain time period,
 *  pump will raise an alarm and alarm mode is activated.
 *  Actions on alarm mode can be configured. The different alternatives
 *  are that the Heat pump stops producing hot water (default setting)
 *  and/or reduces the room temperature.
 *
 *  Author Andreas Sundberg (sundberg84) www.mysensors.org
 *  
 *  This is a fork and development from NibeGW in openhab.
 *  Original Author: pauli.anttila@gmail.com
 *  2.11.2013	v1.00	Initial version.
 *  3.11.2013   v1.01   
 */
 
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <MySensors.h>
#include <SPI.h> 
#include <avr/wdt.h>

#define SKETCH_NAME "Nibe Heatpump Sensor"
#define SKETCH_MAJOR_VER "1"
#define SKETCH_MINOR_VER "0"

// direction change pin for RS-485 port
#define directionPin  2

#ifdef MY_DEBUG
char verbose = 0;
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

// ######### SETUP #######################

void setup()  {
  
  pinMode(directionPin, OUTPUT);
  digitalWrite(directionPin, LOW);
  
  Serial.begin(9600, SERIAL_8N1);
  
  #ifdef MY_DEBUG
  if (verbose) {
    debugPrint("\nstarted\n");
  }
  #endif
}

// ######### MAIN LOOP #######################

void loop() {
  
  switch(state) {
  
    case STATE_WAIT_START:
      if (Serial.available() > 0) {
        byte b = Serial.read();
  
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
      if (Serial.available() > 0) {
        byte b = Serial.read();
  
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
          
          #ifdef MY_DEBUG
          if (verbose > 1) {
            sprintf(debug_buf, "\ncheckMsg=%d\n", msglen);
            debugPrint(debug_buf);
          }
          #endif
              
          switch (msglen)
          {
            case 0:   break; // Ok, but not ready
            case -1:  state = STATE_WAIT_START; break; // Invalid message
            case -2:  state = STATE_CRC_FAILURE; break; // Checksum error
            default:  state = STATE_OK_MESSAGE_RECEIVED; break;
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
        sendUdpPacket(buffer, index);
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
    #ifdef MY_DEBUG
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
            
            #ifdef MY_DEBUG
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
  Serial.write(0x06);
  Serial.flush();
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
  Serial.write(0x15);
  Serial.flush();
  delay(1);
  digitalWrite(directionPin, LOW);
}

void sendUdpPacket(const byte* const data, int len) {
  
  #ifdef MY_DEBUG
  if (verbose) {
    sprintf(debug_buf, "Sending UDP packet, len=%d\n", len);
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
  
  udp.beginPacket(target_ip, udp_port);
  udp.write(data, len);
  udp.endPacket();
}

#ifdef MY_DEBUG
void debugPrint(char* data) {
  udp.beginPacket(target_ip, 50000);
  udp.write(data);
  udp.endPacket();
}
#endif
