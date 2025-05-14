cmake_minimum_required(VERSION 3.10)
project(midiGlow)

set(CMAKE_CXX_STANDARD 17)

# Include paths
include_directories(src include)
include_directories(external/rpi_ws281x)

# Add subdirectories for libraries
add_subdirectory(libs/MidiToLed)

# Add rpi_ws281x subdirectory
add_subdirectory(external/rpi_ws281x)

# Add RtMidi manually (as STATIC)
add_subdirectory(external/rtmidi)


# Specify source files
add_executable(midiGlow src/main.cpp)

# Link rtmidi and ws281x
target_link_libraries(midiGlow PRIVATE
    rtmidi
    ws2811
)

# Make sure to add linker flags for libraries if needed
if (NOT WIN32)
    target_link_libraries(midiGlow PRIVATE
        rt
    )
endif()
