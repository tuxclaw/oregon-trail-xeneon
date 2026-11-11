# Oregon Trail — Xeneon Edge Edition

A classic Oregon Trail game written in **C++ and Qt**, specifically designed to fit perfectly on the **Corsair Xeneon Edge** (14.5" 2560×720 ultrawide touchscreen).

## Features
- Optimized 32:9 layout for 2560×720
- Touch-friendly large controls
- Classic Oregon Trail mechanics (based on the publicly published 1978 BASIC source)
- Party status, supplies, hunting, events, river crossings, and more
- Clean Qt Widgets UI with custom trail visualization

## Requirements
- Qt 6 (or Qt 5.15+)
- CMake 3.16+
- C++17 compiler

## Build
```bash
git clone https://github.com/tuxclaw/oregon-trail-xeneon.git
cd oregon-trail-xeneon
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64   # adjust as needed
cmake --build .
./oregon-trail-xeneon
```

On the Xeneon Edge, run the binary fullscreen or windowed at 2560×720.

## Controls
- Touch / mouse: large action buttons
- Keyboard: number keys and arrows also work

## License
MIT. Gameplay inspired by the original Oregon Trail whose source was published in *Creative Computing* (1978).
