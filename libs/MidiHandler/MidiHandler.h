#pragma once

#include <cstdint>
#include <RtMidi.h>
#include <vector>
typedef struct
{
    uint8_t note;
    uint8_t velocity;
    bool on;      // true for note on, false for note off
    bool sustain; // true if the note is sustained
} MidiNoteEvent;

class MidiHandler
{
private:
    RtMidiIn *midiIn;
    RtMidiOut *midiOut;
    bool passThroughMode;

public:
    MidiHandler(bool passThroughMode = false);
    ~MidiHandler();
    void setPassThroughMode(bool passThroughMode);
    bool getPassThroughMode() const;
    int init();
};
