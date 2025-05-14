project(midiGlow)

set(SOURCES
    MidiToLed.cpp
)

add_library(midi_to_led STATIC ${SOURCES})
