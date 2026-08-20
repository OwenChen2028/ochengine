# Ochengine

Ochengine is a small, header-only 2D physics engine written in C++11. SFML is
used for the window, input, and rendering.

It currently supports:

- Rectangle/rectangle, circle/circle, and rectangle/circle collisions
- Collision resolution with restitution
- Position correction to prevent object penetration
- Symplectic Euler and RK2 integration
- Per-object mass, velocity, gravity, and force

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
