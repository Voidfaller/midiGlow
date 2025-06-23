#include <MidiHandler.h>
int main()
{

    MidiHandler midiHandler;

    midiHandler.setConfigFilePath("../config/config.json");
    midiHandler.init();
    while (true)
    {
        midiHandler.updateNotes();
    }

    return 0;
}