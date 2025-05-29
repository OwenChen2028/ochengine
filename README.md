# Ochengine

Ochengine © 2024 by Owen Chen is licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/?ref=chooser-v1)

Ochengine is a custom 2D physics engine implemented in C++ with SFML rendering support. Notably, no other libraries are used. It supports rigid body physics simulation with collision detection and resolution for rectangles and circles. 

## Overview

The following features have been implemented:
- Rigid body dynamics with position and velocity updates
- Multiple integration methods (Symplectic Euler and RK4)
- Collision detection between various shape combinations:
  - Rectangle to rectangle
  - Circle to circle
  - Rectangle to circle
- Collision resolution with impulse-based physics
- Position correction to prevent object penetration

## Code Structure

The project is organized into three main files:

### 1. Objects&Collisions.h

Contains the core physics engine components:
- Basic mathematical utility functions
- Object base class for physics entities
- Rect and Circle shape implementations
- Collision detection algorithms
- Collision resolution algorithms

### 2. Scenes&Rendering.h

Provides scene management and rendering capabilities:
- Dynamic container implementation
- Scene management
- Game loop implementation
- SFML integration for rendering
- Event handling

### 3. EngineDemo.cpp

Demonstrates the engine capabilities with two demo applications:
- Physics simulation with multiple objects
- Interactive Pong game with keyboard controls

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

## Interactive Demos

### Simulation Demo

Demonstrates various physics interactions with multiple objects of different shapes and masses interacting with gravity and boundaries.

### Pong Demo

An interactive implementation of the classic Pong game with:
- Keyboard controls (W/S for left paddle, Up/Down arrows for right paddle)
- Physics-based ball movement
- Collision response

## How to Use

### Requirements

- C++ compiler with C++11 support
- SFML library

### Running the Demos

1. Compile the project with your C++ compiler, linking against SFML
2. Run the executable
3. Use the menu to select a demo:
   - Enter `0` for the simulation demo
   - Enter `1` for the interactive pong demo
   - Enter `2` to reload the scenes
   - Enter `-1` to exit

### Extending the Engine
The engine can be extended by creating new shape types (inheriting from the Object class), implementing collision detection for these shapes, adding support for tags, etc. Feel free to explore and build upon this foundation to create your own unique games and physics simulations. 
