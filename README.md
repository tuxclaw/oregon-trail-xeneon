# Oregon Trail — Xeneon Edge Edition

A classic Oregon Trail game written in **C++ and Qt**, specifically designed to fit perfectly on the **Corsair Xeneon Edge** (14.5" 2560×720 ultrawide touchscreen).

## Features
- Optimized 32:9 layout for 2560×720 with touch-friendly controls
- Occupation selection + custom party names
- Hunting minigame (timing-based shoot)
- River crossing choices (ford / caulk / ferry)
- Improved trail graphics with landmarks (Chimney Rock, Fort Laramie, etc.)
- High score tracking (saved via QSettings)
- Optional sound effects
- Classic Oregon Trail mechanics (inspired by the publicly published 1978 source)

## Requirements
- Qt 6 (Widgets + Multimedia)
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

Run fullscreen or at 2560×720 on the Xeneon Edge.

## Optional Sounds
Place `.wav` files in a `sounds/` folder next to the binary:
- `travel.wav`, `hunt.wav`, `event.wav`, `win.wav`, `lose.wav`, `shot.wav`, `river.wav`

## License
MIT. Gameplay inspired by the original Oregon Trail whose source was published in *Creative Computing* (1978).
