add_library(MidiHandler MidiHandler.cpp)
target_include_directories(MidiHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MidiHandler PUBLIC rtmidi)