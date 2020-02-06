// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24
#define MY_RF24_PA_LEVEL RF24_PA_MAX

#define MY_NODE_ID 100
#define MY_PARENT_NODE_ID 0
#define MY_REPEATER_FEATURE

#define MY_SOFTSPI
#define MY_SOFT_SPI_SCK_PIN 14
#define MY_SOFT_SPI_MISO_PIN 16
#define MY_SOFT_SPI_MOSI_PIN 15


// When W5100 is connected we have to move CE/CSN pins for NRF radio
#ifndef MY_RF24_CE_PIN
#define MY_RF24_CE_PIN 5
#endif
#ifndef MY_RF24_CS_PIN
#define MY_RF24_CS_PIN 6
#endif

#include <MySensors.h>

void setup()  
{  
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Repeater #100", "1.2");
}

void loop()     
{
     
}
