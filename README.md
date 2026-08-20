# Ochengine

Ochengine is a small, header-only 2D physics engine written in C++11 without the
standard library. It uses SFML for windows, input, and drawing. The container
and math routines, including square root, are implemented in the engine.

## Features

- Circle and axis-aligned rectangle collision detection
- Impulse-based collision response with restitution and position correction
- Symplectic Euler and midpoint RK2 integration
- Scenes with event, update, collision, and drawing hooks

## Build

Requirements:

- C++ compiler with C++11 support
- CMake 3.16 or newer
- SFML 2.5 or newer

On Ubuntu, SFML can be installed with `sudo apt install libsfml-dev`.

```bash
cmake -S . -B build
cmake --build build
```

Pass `-DOCHENGINE_BUILD_EXAMPLES=OFF` to CMake to skip the examples.

## Examples

```bash
./build/simulation_demo
./build/pong_demo
```

Press R to restart either demo. In Pong, W/S controls the left paddle and the
arrow keys control the right paddle. Close the window to quit.

## Use as a library

Add `include/` to your include path, then include either the complete engine:

```cpp
#include <ochengine/ochengine.hpp>
```

or only the headers needed by your program. The CMake target is
`ochengine::ochengine` when the project is included with `add_subdirectory`.

Derive from `Scene` and add an instance to `game.scenes`. `PlayScene` takes the
scene's index. Objects go in the scene's `objects` container. `Game` owns its
scenes, and `Scene` owns its objects.

The available hooks are:

- `HandleEvent` for SFML input events
- `HandleUpdates` once per frame
- `HandleFixedUpdate` before each physics step
- `OnCollisionStay` after a collision is resolved
- `HandleDraw` for drawing the frame

The default `HandleDraw` clears the window and draws every active object in
white. An override replaces that behavior.
