#pragma once

#include <vector>
#include <cstdint>
#include <RtMidi.h>
#include "../rpi_ws281x/ws2811.h"
#include <MidiHandler.h>
class IActuator
{
    public:
        virtual ~IActuator() = default; // Virtual destructor for proper cleanup
        virtual int init() = 0; // Initialize the actuator
        virtual void onNoteOn(uint8_t note, uint8_t velocity) = 0
        virtual void onNoteOff(uint8_t note) = 0; // Handle note on and note off events

    protected:
        MidiNoteEvent noteEvents[128]
};