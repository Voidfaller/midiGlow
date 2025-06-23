cmake_minimum_required(VERSION 3.16)
project(midiGlow)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/IActuator)
add_subdirectory(libs/MidiHandler)
add_subdirectory(external/rtmidi)
add_subdirectory(external/rpi_ws281x)
add_subdirectory(libs/SK6812)

add_executable(midiGlow src/main.cpp)

target_link_libraries(midiGlow
    IActuator
    MidiHandler
    SK6812
)