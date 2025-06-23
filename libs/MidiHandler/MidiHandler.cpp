#include <MidiHandler.h>
#include <RtMidi.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <time.h>
// defines
// MIDI Command Constants
constexpr uint8_t MIDI_NOTE_OFF = 0x80;
constexpr uint8_t MIDI_NOTE_ON = 0x90;
constexpr uint8_t MIDI_POLY_PRESSURE = 0xA0;
constexpr uint8_t MIDI_CONTROL_CHANGE = 0xB0;
constexpr uint8_t MIDI_PROGRAM_CHANGE = 0xC0;
constexpr uint8_t MIDI_CHANNEL_PRESSURE = 0xD0;
constexpr uint8_t MIDI_PITCH_BEND = 0xE0;
constexpr uint8_t MIDI_SYSTEM_EXCLUSIVE = 0xF0;
constexpr uint8_t MIDI_TIME_CODE = 0xF1;
constexpr uint8_t MIDI_SONG_POSITION = 0xF2;
constexpr uint8_t MIDI_SONG_SELECT = 0xF3;
constexpr uint8_t MIDI_TUNE_REQUEST = 0xF6;
constexpr uint8_t MIDI_END_OF_EXCLUSIVE = 0xF7;
constexpr uint8_t MIDI_TIMING_CLOCK = 0xF8;
constexpr uint8_t MIDI_START = 0xFA;
constexpr uint8_t MIDI_CONTINUE = 0xFB;
constexpr uint8_t MIDI_STOP = 0xFC;
constexpr uint8_t MIDI_ACTIVE_SENSING = 0xFE;
constexpr uint8_t MIDI_SYSTEM_RESET = 0xFF;

// constructor
MidiHandler::MidiHandler()
{
    midiIn = new RtMidiIn();
    midiOut = new RtMidiOut();
}

// destructor
MidiHandler::~MidiHandler()
{
    if (midiIn)
    {
        delete midiIn;
        midiIn = nullptr;
    }
    if (midiOut)
    {
        delete midiOut;
        midiOut = nullptr;
    }
}

int MidiHandler::loadConfig(const std::string &configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Error opening config file: " << configFilePath << std::endl;
        return -1;
    }

    nlohmann::json config;
    configFile >> config;

    // Load MIDI input and output ports
    midiInputPort = config["midiInPort"];
    midiOutputPort = config["midiOutPort"];

    // Load passthrough mode
    passThroughMode = config["passThroughMode"];

    return 0;
}

int MidiHandler::setConfigFilePath(const std::string &configFilePath)
{
    this->configFilePath = configFilePath;
    return 0;
}

void MidiHandler::handleMidiMessage(const std::vector<unsigned char> &message)
{
    if (message.size() < 2)
        return; // safety check

    uint8_t status = message[0];
    uint8_t command = status & 0xF0;
    uint8_t channel = status & 0x0F;

   if (command == MIDI_NOTE_ON)
   {
    MidiNoteEvent noteEvent;
    noteEvent.note = message[1];
    noteEvent.velocity = message[2];  
    noteEvent.startTime = static_cast<double>(clock()) / CLOCKS_PER_SEC; // Get current time in seconds
    noteEvent.phase = EnvelopePhase::Attack; // Set the phase to Attack
    activeNotes[noteEvent.note] = noteEvent; // Store the note event in the active notes map  
   }
   else if (command == MIDI_NOTE_OFF){
    std::cout << "Note Off: Channel " << (int)channel << ", Note " << (int)message[1] << ", Velocity " << (int)message[2] << std::endl;
    auto it = activeNotes.find(message[1]);
    if (it != activeNotes.end())
    {
        MidiNoteEvent &noteEvent = it->second;
        noteEvent.phase = EnvelopePhase::Release; // Set the phase to Release
        noteEvent.startTime = static_cast<double>(clock()) / CLOCKS_PER_SEC; // Update the start time
        std::cout << "Note released: " << (int)noteEvent.note << ", Phase: " << static_cast<int>(noteEvent.phase) << std::endl;
    }
   }
   else if (command == MIDI_CONTROL_CHANGE)
   {
       std::cout << "Control Change: Channel " << (int)channel << ", Controller " << (int)message[1] << ", Value " << (int)message[2] << std::endl;
   }
   else
   {
       std::cout << "Other MIDI Message: Status " << (int)status << std::endl;
   }

    std::cout << std::endl;

    // debug output
    std::cout << "Active Notes:" << std::endl;
    for (const auto &pair : activeNotes)
    {
        const MidiNoteEvent &noteEvent = pair.second;
        std::cout << "Note: " << (int)noteEvent.note
                  << ", Velocity: " << (int)noteEvent.velocity
                  << ", Start Time: " << noteEvent.startTime
                  << ", Phase: " << static_cast<int>(noteEvent.phase) << std::endl;
    }
    std::cout << "------------------------" << std::endl;
}

void MidiHandler::updateNotes()
{
    double now = static_cast<double>(clock()) / CLOCKS_PER_SEC; // Get current time in seconds
    for (auto it = activeNotes.begin(); it != activeNotes.end();)
    {
        MidiNoteEvent &noteEvent = it->second;

        // Check if the note is in the Release phase and has been active for more than 1 second
        if (noteEvent.phase == EnvelopePhase::Release && (now - noteEvent.startTime) > 1.0)
        {
            std::cout << "Note " << (int)noteEvent.note << " has been released for more than 1 second, removing from active notes." << std::endl;
            it = activeNotes.erase(it); // Remove the note from active notes
        }
        else
        {
            ++it; // Move to the next note
        }
    }
}

void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
    if (message->empty())
        return;

    // Optional: if passthrough is enabled, send it to output
    MidiHandler *handler = static_cast<MidiHandler *>(userData);

    handler->handleMidiMessage(*message);

    if (handler->isPassThroughMode())
    {
        handler->sendMidiMessage(*message);
    }
}

void MidiHandler::sendMidiMessage(const std::vector<unsigned char> &message)
{
    if (midiOut && midiOut->isPortOpen())
    {
        midiOut->sendMessage(&message);
    }
}
int MidiHandler::init()
{

    if (this->configFilePath.empty())
    {
        std::cerr << "Config file path is empty. Please set the config file path before initializing." << std::endl;
        return -1;
    }

    loadConfig(this->configFilePath);

    // Open MIDI input and output ports
    midiIn->openPort(midiInputPort);
    midiIn->ignoreTypes(false, false, false);
    midiIn->setCallback(&midiCallback, this); // Set the callback function for MIDI input
    midiOut->openPort(midiOutputPort);

    std::cout << "MIDI input port: " << midiInputPort << std::endl;
    std::cout << "MIDI output port: " << midiOutputPort << std::endl;
    std::cout << "MIDI passthrough mode: " << (passThroughMode ? "enabled" : "disabled") << std::endl;

    return 0;
}
