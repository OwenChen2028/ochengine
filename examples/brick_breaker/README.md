# Brick Breaker

A brick-breaker game built with Ochengine. Build it from this directory.

## Build

Requirements:

- C++ compiler with C++11 support
- CMake 3.16 or newer
- SFML 2.5 or newer

From this directory:

```bash
cmake -S . -B build
cmake --build build
./build/brick_breaker
```

## Controls

- A/D or Left/Right: move the paddle
- Space: launch the ball
- R: restart the game

## Gameplay

You start with three lives and 40 bricks. Clearing the board starts the next
level with a faster launch speed. The window title shows the score, remaining
lives, and current level.
