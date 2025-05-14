#include <MidiHandler.h>
#include <RtMidi.h>
#include <nlohmann/json.hpp>
#include <iostream>
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

int MidiHandler::init(){
    midiIn->openPort(midiInputPort);
    midiOut->openPort(midiOutputPort);

    std::cout << "MIDI input port: " << midiInputPort << std::endl;
    std::cout << "MIDI output port: " << midiOutputPort << std::endl;
    std::cout << "MIDI passthrough mode: " << (passThroughMode ? "enabled" : "disabled") << std::endl;
    return 0;
}