#pragma once

enum class IntegrationType { Euler, RK2 };

enum class ShapeType { Rectangle, Circle };

struct Object {
  float mass;
  float invMass;

  float restitution;

  float velocityX;
  float velocityY;

  float gravity;

  ShapeType shape;
  bool active;

  float forceX;
  float forceY;

  Object(float mass_, float restitution_, float velocityX_, float velocityY_, float gravity_, ShapeType shape_) {
    mass = mass_;

    if (mass != 0) {
      invMass = 1 / mass;
    } else {
      invMass = 0; // infinite mass
    }

    restitution = restitution_;

    velocityX = velocityX_;
    velocityY = velocityY_;

    gravity = gravity_;

    shape = shape_;
    active = true;

    forceX = 0.0f;
    forceY = 0.0f;
  }

  virtual ~Object() = default;

  virtual float GetCenterX() = 0;
  virtual float GetCenterY() = 0;

  virtual void Move(float dx, float dy) = 0;

  void PhysicsUpdate(float dt, IntegrationType method) {
    forceY += mass * gravity;

    if (method == IntegrationType::Euler) { // symplectic euler
      velocityX += invMass * forceX * dt;
      velocityY += invMass * forceY * dt;

      Move(velocityX * dt, velocityY * dt);
    } else if (method == IntegrationType::RK2) { // midpoint runge kutta 2
      float accelX = invMass * forceX;
      float accelY = invMass * forceY;

      float midVelocityX = velocityX + accelX * dt / 2.0f;
      float midVelocityY = velocityY + accelY * dt / 2.0f;

      Move(midVelocityX * dt, midVelocityY * dt);

      velocityX += accelX * dt;
      velocityY += accelY * dt;
    }

    forceX = 0.0f;
    forceY = 0.0f;
  };
};
