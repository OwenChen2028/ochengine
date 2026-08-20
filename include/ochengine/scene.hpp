#pragma once

#include "ochengine/collision.hpp"
#include "ochengine/container.hpp"
#include <SFML/Graphics.hpp>

struct Scene {
  Container<Object *> objects; // todo: use diff data structure (e.g. quadtree)

  Scene(Container<Object *> objects_ = Container<Object *>()) { objects = objects_; }

  Scene(const Scene &) = delete;
  Scene &operator=(const Scene &) = delete;

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

        Object *object1 = objects.getValue(i);
        Object *object2 = objects.getValue(j);
        bool collision = false;
        bool swapped = false;

        if (object1->shape == ShapeType::Circle && object2->shape == ShapeType::Rectangle) {
          Object *temp = object1;
          object1 = object2;
          object2 = temp;
          swapped = true;
        }

        Collision col(object1, object2);

        if (object1->shape == ShapeType::Rectangle && object2->shape == ShapeType::Rectangle) {
          collision = CheckRectRectCol(&col);
        } else if (object1->shape == ShapeType::Rectangle && object2->shape == ShapeType::Circle) {
          collision = CheckRectCircleCol(&col);
        } else if (object1->shape == ShapeType::Circle && object2->shape == ShapeType::Circle) {
          collision = CheckCircleCircleCol(&col);
        }

        if (collision) {
          ResolveCollision(&col);
          CorrectPositions(&col);

          if (swapped) {
            Object *temp = col.object1;
            col.object1 = col.object2;
            col.object2 = temp;

            col.normalX *= -1;
            col.normalY *= -1;
          }

          OnCollisionStay(col);
        }
      }
    }
  }

  virtual void HandleEvent(sf::Event) {}
  virtual void HandleUpdates() {}
  virtual void HandleFixedUpdate(float) {}
  virtual void OnCollisionStay(Collision &) {}

  virtual void HandleDraw(sf::RenderWindow &window) {
    window.clear();

    sf::RectangleShape rectShape;
    sf::CircleShape circleShape;

    for (int i = 0; i < objects.getSize(); i++) {
      if (!objects.getValue(i)->active) {
        continue;
      }

      if (objects.getValue(i)->shape == ShapeType::Rectangle) {
        Rect *rect = static_cast<Rect *>(objects.getValue(i));

        rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
        rectShape.setPosition(rect->minX, rect->minY);
        rectShape.setFillColor(sf::Color::White);

        window.draw(rectShape);
      } else if (objects.getValue(i)->shape == ShapeType::Circle) {
        Circle *circle = static_cast<Circle *>(objects.getValue(i));

        circleShape.setRadius(circle->radius);
        circleShape.setPosition(circle->posX - circle->radius, circle->posY - circle->radius);
        circleShape.setFillColor(sf::Color::White);

        window.draw(circleShape);
      }
    }
  }
};
