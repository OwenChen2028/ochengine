# Ochengine

Ochengine is a custom 2D physics engine implemented in C++ with SFML rendering support. Notably, no other libraries are used.

## Overview

The following features have been implemented:

- Translational rigid-body dynamics
- Multiple integration methods (Symplectic Euler and RK4)
- Collision detection and resolution between various shapes:
  - Rectangle to rectangle
  - Circle to circle
  - Rectangle to circle
- Position correction to prevent object penetration

## Project Structure

```text
ochengine/
├── CMakeLists.txt
├── include/
│   └── ochengine/
│       ├── math.hpp                # Math helpers
│       ├── object.hpp              # Base physics object and integration
│       ├── shapes.hpp              # Rectangle and circle objects
│       ├── collision.hpp           # Collision detection and resolution
│       ├── container.hpp           # Dynamic container template
│       ├── scene.hpp               # Scene ownership and physics updates
│       ├── game.hpp                # SFML rendering and game loop
│       └── ochengine.hpp           # Complete convenience header
└── examples/
    └── engine_demo.cpp             # Simulation and Pong demos
```

Ochengine remains header-only, so applications can include the specific component
they need or use `#include <ochengine/ochengine.hpp>` for the complete engine.

## Physics Features

### Integration Methods

The engine supports two numerical integration methods:

- **Symplectic Euler**: Simple and fast, but less accurate
- **Runge-Kutta 4 (RK4)**: More accurate for complex motion

### Collision Detection

The engine implements specialized collision detection for different shape combinations:

- **Rectangle-Rectangle**: Separating axis theorem
- **Circle-Circle**: Distance-based detection
- **Rectangle-Circle**: Closest point approach

### Physics Properties

Each object can have the following physical properties:

- **Mass**: Determines how forces affect the object
- **Inverse Mass**: Used for efficient calculations (infinite mass when zero)
- **Restitution**: Controls bounciness in collisions
- **Velocity**: Vector representing speed and direction
- **Gravity**: Individual gravity settings
- **Force**: Accumulator for applied forces

## How to Use

### Requirements

- C++ compiler with C++11 support
- CMake 3.16 or newer
- SFML library: `sudo apt install libsfml-dev`

### Building

```bash
cmake -S . -B build
cmake --build build
```

Set `OCHENGINE_BUILD_EXAMPLES=OFF` if the demo executable is not needed.

### Running the Demo

```bash
./build/ochengine_demo
```

Use the menu to select a demo:

- Enter `0` for the simulation demo
- Enter `1` for the interactive pong demo
- Enter `2` to reload the scenes
- Enter `-1` to exit
