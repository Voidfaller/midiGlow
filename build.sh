#!/bin/bash

RUN_FLAG=false

for arg in "$@"; do
  if [ "$arg" == "--run" ]; then
    RUN_FLAG=true
  fi
done


mkdir -p build
cd build
cmake ..
make

if [ $? -eq 0 ]; then
  echo "Build successful!"

    if [ "$RUN_FLAG" = true ]; then
        echo "Running the program..."
        cd build
        ./midiGlow
         # Replace with your actual program name
    else
        echo "Use --run to execute the program."
    fi
else
  echo "Build failed!"
  exit 1
fi