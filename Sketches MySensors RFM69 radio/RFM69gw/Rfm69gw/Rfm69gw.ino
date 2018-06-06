#include <SPI.h>

// Enable debug prints to serial monitor
#define MY_DEBUG
#define MY_DEBUG_VERBOSE_SIGNING

// Enable and select radio type attached
//#define MY_RADIO_NRF24
#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY RFM69_433MHZ
#define MY_IS_RFM69HW

//Signing
#define MY_SIGNING_ATSHA204
#define MY_SIGNING_WEAK_SECURITY //< currently to allow not signing-upgraded nodes to continue communicate. Will be removed once all nodes atsha204

// Enable serial gateway
#define MY_GATEWAY_SERIAL

// Define a lower baud rate for Arduino's running on 8 MHz (Arduino Pro Mini 3.3V & SenseBender)
#if F_CPU == 8000000L
#define MY_BAUD_RATE 38400
#endif

#include <MySensors.h>

void setup()
{
    // Setup locally attached sensors
}

void presentation()
{
    // Present locally attached sensors
}

void loop()
{
    // Send locally attached sensor data here
}
