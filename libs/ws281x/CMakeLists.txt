project(midiGlow)

set(SOURCES
    ws281x.cpp
)

add_library(ws281x STATIC ${SOURCES})
