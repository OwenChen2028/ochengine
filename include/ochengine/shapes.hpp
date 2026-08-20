#pragma once

#include "ochengine/object.hpp"

struct Rect : Object {
  float minX;
  float minY;

  float maxX;
  float maxY;

  Rect(float mass_, float restitution_, float velocityX_, float velocityY_, float gravity_, float minX_, float minY_, float maxX_, float maxY_) : Object(mass_, restitution_, velocityX_, velocityY_, gravity_, ShapeType::Rectangle) {
    minX = minX_;
    minY = minY_;

    maxX = maxX_;
    maxY = maxY_;
  }

  float GetCenterX() override { return (minX + maxX) / 2; }

  float GetCenterY() override { return (minY + maxY) / 2; }

  void Move(float dx, float dy) override {
    minX += dx;
    maxX += dx;

    minY += dy;
    maxY += dy;
  }
};

struct Circle : Object {
  float radius;

  float posX;
  float posY;

  Circle(float mass_, float restitution_, float velocityX_, float velocityY_, float gravity_, float radius_, float posX_, float posY_) : Object(mass_, restitution_, velocityX_, velocityY_, gravity_, ShapeType::Circle) {
    radius = radius_;

    posX = posX_;
    posY = posY_;
  }

  float GetCenterX() override { return posX; }

  float GetCenterY() override { return posY; }

  void Move(float dx, float dy) override {
    posX += dx;
    posY += dy;
  }
};
