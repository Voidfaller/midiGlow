#include <ws281x.h>

Ws2812Actuator::Ws2812Actuator(int ledCount, int gpioPin)
    : ledCount(ledCount), gpioPin(gpioPin), ledStrip(), ledColors(ledCount, 0) {
    ledStrip.freq = WS2811_TARGET_FREQ;
    ledStrip.dmanum = 10; // Use DMA channel 10
    ledStrip.channel[0].gpionum = gpioPin;
    ledStrip.channel[0].count = ledCount;
    ledStrip.channel[0].invert = 0;
    ledStrip.channel[0].brightness = 255;
    ledStrip.channel[0].strip_type = WS2811_STRIP_GRB; // Use GRB color order
}