#pragma once

#include <cstdint>
#include <RtMidi.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>

enum class EnvelopePhase
{
    Attack,
    Decay,
    Sustain,
    Release,
    Off
};
struct MidiNoteEvent
{
    uint8_t note;
    uint8_t velocity;
    double startTime;
    EnvelopePhase phase;
};

inline std::unordered_map<uint8_t, MidiNoteEvent> activeNotes;
class MidiHandler
{
public:
    MidiHandler();                                                     // Constructor with passThroughMode parameter
    ~MidiHandler();                                                    // Destructor
    int init();                                                        // Initialize MIDI input and output
    int setConfigFilePath(const std::string &configFilePath);          // Set configuration file path
    bool isPassThroughMode() const { return passThroughMode; }         // Check if passthrough mode is enabled
    void sendMidiMessage(const std::vector<unsigned char> &message);   // Send MIDI message
    void handleMidiMessage(const std::vector<unsigned char> &message); // MIDI output port number
    void updateNotes();                                                // Update active notes based on their phases
private:
    std::string configFilePath;                        // Path to the configuration file
    RtMidiIn *midiIn = nullptr;                        // MIDI input object
    RtMidiOut *midiOut = nullptr;                      // MIDI output object
    bool passThroughMode = false;                      // MIDI passthrough mode flag
    int loadConfig(const std::string &configFilePath); // Load configuration from file
    int midiInputPort = 0;                             // MIDI input port number
    int midiOutputPort = 0;
};
