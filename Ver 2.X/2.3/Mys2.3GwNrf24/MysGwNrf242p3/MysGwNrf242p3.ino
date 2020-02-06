// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
#define MY_RF24_PA_LEVEL RF24_PA_MAX

// Enable gateway ethernet module type
#define MY_GATEWAY_W5100

// Enable Soft SPI for NRF radio (note different radio wiring is required)
// The W5100 ethernet module seems to have a hard time co-operate with
// radio on the same spi bus.
#if !defined(MY_W5100_SPI_EN) && !defined(ARDUINO_ARCH_SAMD)
#define MY_SOFTSPI
#define MY_SOFT_SPI_SCK_PIN 14
#define MY_SOFT_SPI_MISO_PIN 16
#define MY_SOFT_SPI_MOSI_PIN 15
#endif

// When W5100 is connected we have to move CE/CSN pins for NRF radio
#ifndef MY_RF24_CE_PIN
#define MY_RF24_CE_PIN 5
#endif
#ifndef MY_RF24_CS_PIN
#define MY_RF24_CS_PIN 6
#endif

#define MY_IP_ADDRESS 192,168,1,8   // If this is disabled, DHCP is used to retrieve address
// Renewal period if using DHCP
//#define MY_IP_RENEWAL_INTERVAL 60000
// The port to keep open on node server mode / or port to contact in client mode
#define MY_PORT 5003      

// Controller ip address. Enables client mode (default is "server" mode). 
// Also enable this if MY_USE_UDP is used and you want sensor data sent somewhere. 
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 178, 254   
 
// The MAC address can be anything you want but should be unique on your network.
// Newer boards have a MAC address printed on the underside of the PCB, which you can (optionally) use.
// Note that most of the Ardunio examples use  "DEAD BEEF FEED" for the MAC address.
#define MY_MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x08 //AF-A0-F2-15-3B-1C

// Set blinking period
#define MY_DEFAULT_LED_BLINK_PERIOD 300

// Enable inclusion mode
//#define MY_INCLUSION_MODE_FEATURE    
// Enable Inclusion mode button on gateway
//#define MY_INCLUSION_BUTTON_FEATURE
// Set inclusion mode duration (in seconds)
//#define MY_INCLUSION_MODE_DURATION 60 
// Digital pin used for inclusion mode button
//#define MY_INCLUSION_MODE_BUTTON_PIN  3 

// Uncomment to override default HW configurations
#define MY_DEFAULT_ERR_LED_PIN 7  // Error led pin
#define MY_DEFAULT_RX_LED_PIN  9  // Receive led pin
#define MY_DEFAULT_TX_LED_PIN  8  // the PCB, on board LED

#include <SPI.h>

#if defined(MY_USE_UDP)
  #include <EthernetUdp.h>
#endif
#include <Ethernet.h>
#include <MySensors.h>


void setup()
{
}

void loop() {
}
