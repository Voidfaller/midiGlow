#include <iostream>
#include <unistd.h>
#include <RtMidi.h>
#include "../rpi_ws281x/ws2811.h"

#define LED_COUNT 120
#define GPIO_PIN 18
#define DMA 10

ws2811_t ledstring = {
    .freq = WS2811_TARGET_FREQ, // Set the frequency for WS2811
    .dmanum = DMA,              // Set DMA channel number
    .channel = {
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = LED_COUNT,
            .strip_type = SK6812_STRIP, // Set the strip type (RGB)
            .leds = nullptr,            // LED buffer will be allocated by the driver
            .brightness = 255,          // Set brightness (0-255)
            .wshift = 0,                // White shift value
            .rshift = 0,                // Red shift value
            .gshift = 0,                // Green shift value
            .bshift = 0,                // Blue shift value
            .gamma = nullptr,           // Gamma correction table (optional)
        }}};

// Function to update LEDs based on MIDI input
// Function to update LEDs based on MIDI input
void updateLEDs(int midiNote, int velocity)
{
    printf("Updating LED for MIDI note: %d with velocity: %d\n", midiNote, velocity);

    // Ensure midiNote is within the bounds of the LED array (0 to LED_COUNT - 1)
    int ledIndex = midiNote % LED_COUNT;

    // Normalize note to 0.0–1.0 based on your range
    float normalized = (midiNote - 36) / 60.0f; // 60 = 96 - 36

    // Convert to RGB using HSV (Hue-Saturation-Value) model
    // Hue rotates through colors — good for even distribution
    float hue = normalized * 360.0f; // Full color wheel (0-360 degrees)

    int red, green, blue;

    // Convert hue to RGB
    int h = int(hue / 60.0f) % 6;
    float f = (hue / 60.0f) - h;
    float v = 1.0f;
    float s = 1.0f;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (h)
    {
    case 0:
        red = v * 255;
        green = t * 255;
        blue = p * 255;
        break;
    case 1:
        red = q * 255;
        green = v * 255;
        blue = p * 255;
        break;
    case 2:
        red = p * 255;
        green = v * 255;
        blue = t * 255;
        break;
    case 3:
        red = p * 255;
        green = q * 255;
        blue = v * 255;
        break;
    case 4:
        red = t * 255;
        green = p * 255;
        blue = v * 255;
        break;
    case 5:
        red = v * 255;
        green = p * 255;
        blue = q * 255;
        break;
    }

    int brightness = (velocity * 255) / 127;
    red = (red * brightness) / 255;
    green = (green * brightness) / 255;
    blue = (blue * brightness) / 255;

    int color = (red << 16) | (green << 8) | blue;
    ledstring.channel[0].leds[ledIndex] = color;
    // Render the LED strip with the updated color
    ws2811_render(&ledstring);
}

int main()
{
    // Initialize the LED strip
    if (ws2811_init(&ledstring) != WS2811_SUCCESS)
    {
        std::cerr << "Init failed!" << std::endl;
        return 1;
    }

    // Set up RtMidiIn to receive MIDI input
    RtMidiIn midiIn;

    // Get available MIDI ports
    unsigned int nPorts = midiIn.getPortCount();
    // List all available MIDI ports
    for (unsigned int i = 0; i < nPorts; ++i)
    {
        std::cout << "MIDI Port " << i << ": " << midiIn.getPortName(i) << std::endl;
    }
    midiIn.openPort(1);

    // MIDI message storage
    std::vector<unsigned char> message;

    // Continuously check for MIDI messages
    while (true)
    {
        // Read the message from MIDI input
        midiIn.getMessage(&message);

        if (!message.empty())
        {

            // Check if the message is a Note On
            if (message[0] == 0x90)
            {
                unsigned char midiNote = message[1]; // MIDI note value (0-127)
                unsigned char velocity = message[2]; // Velocity value (0-127)

                updateLEDs(midiNote, velocity);
                std::cout << "Note On: " << (int)midiNote << " Velocity: " << (int)velocity << std::endl;
            }

            else if (message[0] == 0x80) // Note Off message
            {
                unsigned char midiNote = message[1]; // MIDI note value (0-127)
                updateLEDs(midiNote, 0);             // Turn off the LED for this note
                std::cout << "Note Off: " << (int)midiNote << std::endl;
            }
        }

        usleep(10000); // Sleep to avoid high CPU usage
    }

    // Clean up
    ws2811_fini(&ledstring);
    return 0;
}
