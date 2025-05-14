#include <MidiHandler.h>
int main()
{

    MidiHandler midiHandler;

    midiHandler.loadConfig("../config/config.json");
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