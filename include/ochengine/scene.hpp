#pragma once

#include "ochengine/collision.hpp"
#include "ochengine/container.hpp"
#include <SFML/Window/Event.hpp>

struct Scene {
  Container<Object *> objects; // todo: use diff data structure (e.g. quadtree)

  Scene(Container<Object *> objects_ = Container<Object *>()) { objects = objects_; }

  virtual ~Scene() {
    for (int i = 0; i < objects.getSize(); i++) {
      delete objects.getValue(i);
    }
  }

  void HandlePhysicsUpdates(float dt, IntegrationType method) {
    for (int i = 0; i < objects.getSize(); i++) {
      if (objects.getValue(i)->active) {
        objects.getValue(i)->PhysicsUpdate(dt, method);
      }
    }
  }

  void HandleCollisions() {
    for (int i = 0; i < objects.getSize(); i++) {
      for (int j = i + 1; j < objects.getSize(); j++) {
        if (!objects.getValue(i)->active || !objects.getValue(j)->active) {
          continue;
        }

        Collision *col = nullptr;
        bool collision = false;

        if (objects.getValue(i)->shape == ShapeType::Rectangle) {
          if (objects.getValue(j)->shape == ShapeType::Rectangle) {
            col = new Collision(objects.getValue(i), objects.getValue(j));

            if (CheckRectRectCol(col)) {
              collision = true;
            }
          } else if (objects.getValue(j)->shape == ShapeType::Circle) {
            col = new Collision(objects.getValue(i), objects.getValue(j));

            if (CheckRectCircleCol(col)) {
              collision = true;
            }
          }
        } else if (objects.getValue(i)->shape == ShapeType::Circle) {
          if (objects.getValue(j)->shape == ShapeType::Rectangle) {
            col = new Collision(objects.getValue(j),
                                objects.getValue(i)); // swap to rect and circle

            if (CheckRectCircleCol(col)) {
              collision = true;
            }
          } else if (objects.getValue(j)->shape == ShapeType::Circle) {
            col = new Collision(objects.getValue(i), objects.getValue(j));

            if (CheckCircleCircleCol(col)) {
              collision = true;
            }
          }
        }

        if (col != nullptr) {
          if (collision) {
            ResolveCollision(col);
            CorrectPositions(col);
            OnCollision(col);
          }

          delete col;
        }
      }
    }
  }

  virtual void HandleEvent(sf::Event) {}
  virtual void HandleUpdates() {}
  virtual void HandleFixedUpdate(float) {}
  virtual void OnCollision(Collision *) {}
};
