#pragma once

#include <cstdint>
#include <RtMidi.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
typedef struct
{
    uint8_t note;
    uint8_t velocity;
    bool on;      // true for note on, false for note off
    bool sustain; // true if the note is sustained
} MidiNoteEvent;

class MidiHandler
{
public:
    MidiHandler();     // Constructor with passThroughMode parameter
    ~MidiHandler();                                     // Destructor
    int loadConfig(const std::string &configFilePath); // Load configuration from file
    int init();                                     // Initialize MIDI input and output
private:
    RtMidiIn *midiIn = nullptr;   // MIDI input object
    RtMidiOut *midiOut = nullptr; // MIDI output object
    bool passThroughMode = false; // MIDI passthrough mode flag
    int midiInputPort = 0;        // MIDI input port number
    int midiOutputPort = 0;       // MIDI output port number
};
