project(midiGlow)

set(SOURCES
    LedController.cpp
)

add_library(LedController STATIC ${SOURCES})
