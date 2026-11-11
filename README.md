# Oregon Trail — Xeneon Edge Edition

A classic Oregon Trail game written in **C++ and Qt**, specifically designed to fit perfectly on the **Corsair Xeneon Edge** (14.5" 2560×720 ultrawide touchscreen).

## Features
- Proper start menu (New Journey / High Scores / Quit)
- Occupation selection + custom party names
- Hunting minigame (timing-based shoot)
- Multiple river crossings (Kansas, Big Blue, Platte, North Platte, Snake) with ford / caulk / ferry choices
- Dynamic weather system (Clear, Rain, Heavy Rain, Storm, Snow, Blizzard) that affects travel speed, food use, and health
- Weather visuals on the trail (rain, snow, darkened skies)
- Improved trail graphics with landmarks
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
