#include <MidiHandler.h>
int main()
{

    MidiHandler midiHandler;

    midiHandler.setConfigFilePath("../config/config.json");
    midiHandler.init();
    while (true)
    {
        if (std::cin.peek() == 'q')
        {
            break;
        }
        
    }
    
    return 0;
}