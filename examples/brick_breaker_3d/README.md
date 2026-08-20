# Brick Breaker 3D

Brick Breaker played on a table viewed through a fixed perspective camera. All
movement and collision detection still happen in two dimensions. The apparent
height of the walls, bricks, paddle, and ball is only part of the drawing code.

## Build

Requirements:

- C++ compiler with C++11 support
- CMake 3.16 or newer
- SFML 2.5 or newer

From this directory:

```bash
cmake -S . -B build
cmake --build build
./build/brick_breaker_3d
```

## Controls

- A/D or Left/Right: move the paddle
- Space: launch the ball
- R: restart the game

You start with three lives and 40 bricks. Clearing the board starts the next
level with a faster launch speed.
