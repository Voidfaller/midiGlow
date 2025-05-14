#pragma once

#include <vector>
#include <cstdint>
#include <RtMidi.h>
#include "../rpi_ws281x/ws2811.h"

struct MidiNoteEvent {
    uint8_t note;
    uint8_t velocity;
    bool on; // true for note on, false for note off
}