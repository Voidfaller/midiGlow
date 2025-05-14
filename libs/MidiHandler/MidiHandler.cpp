#include <MidiHandler.h>
#include <RtMidi.h>
// constructor
MidiHandler::MidiHandler(bool passThroughMode)
{
    this->passThroughMode = passThroughMode;
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

int MidiHandler::init()
{
    midiIn = new RtMidiIn();
    midiOut = new RtMidiOut();

    midiIn->openPort(0);
    midiOut->openPort(0);

    std::cout  << "MidiHandler initialized" << std::endl;
    return 0;
}