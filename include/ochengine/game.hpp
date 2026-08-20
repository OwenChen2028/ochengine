#pragma once

#include "ochengine/scene.hpp"
#include <SFML/Graphics.hpp>

struct Game {
  sf::RenderWindow window;

  const char *windowName;

  int windowWidth;
  int windowHeight;

  int updateFreq;

  Container<Scene *> scenes;

  Game(const char *windowName_, int windowWidth_, int windowHeight_,
       int updateFreq_ = 60,
       Container<Scene *> scenes_ = Container<Scene *>()) {
    windowName = windowName_;

    windowWidth = windowWidth_;
    windowHeight = windowHeight_;

    updateFreq = updateFreq_;

    scenes = scenes_;
  }

  ~Game() {
    for (int i = 0; i < scenes.getSize(); i++) {
      delete scenes.getValue(i);
    }
  }

  void ToggleWindow(bool open) {
    if (open) {
      window.create(sf::VideoMode(windowWidth, windowHeight), windowName);
      window.setFramerateLimit(updateFreq);
    } else {
      window.close();
    }
  }

  void DrawObjects(Scene *scene) {
    if (!window.isOpen()) {
      return;
    }

    window.clear();

    sf::RectangleShape rectShape;
    sf::CircleShape circleShape;

    for (int i = 0; i < scene->objects.getSize(); i++) {
      if (scene->objects.getValue(i)->shape == ShapeType::Rectangle) {
        Rect *rect = static_cast<Rect *>(scene->objects.getValue(i));

        rectShape.setSize(
            sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
        rectShape.setPosition(rect->minX, rect->minY);
        rectShape.setFillColor(sf::Color::White);

        window.draw(rectShape);
      } else if (scene->objects.getValue(i)->shape == ShapeType::Circle) {
        Circle *circle = static_cast<Circle *>(scene->objects.getValue(i));

        circleShape.setRadius(circle->radius);
        circleShape.setPosition(circle->posX - circle->radius,
                                circle->posY - circle->radius);
        circleShape.setFillColor(sf::Color::White);

        window.draw(circleShape);
      }
    }

    window.display();
  }

  void PlayScene(int sceneId, float duration = 0.0f,
                 IntegrationType method = IntegrationType::RK4,
                 bool deterministic = true, bool waitForFocus = false) {
    Scene *scene = scenes.getValue(sceneId);

    if (waitForFocus) {
      DrawObjects(scene);

      while (!window.hasFocus()) {
        sf::Event event;

        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            window.close();
            break;
          }
        }
      }
    }

    sf::Clock clock;

    float elapsedTime = 0.0f;

    float accumulatedTime = 0.0f;
    float timeStep = 1.0f / updateFreq;

    while (elapsedTime < duration ||
           duration == 0.0f) { // 0 for infinite duration
      sf::Time dt = clock.restart();

      DrawObjects(scene);

      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          ToggleWindow(false);
          break;
        } else {
          scene->ProcessEvent(event);
        }
      }

      if (!window.isOpen()) {
        break;
      }

      scene->HandleUpdates();

      if (deterministic) {
        accumulatedTime += dt.asSeconds();

        while (accumulatedTime >= timeStep) {
          scene->HandlePhysicsUpdates(timeStep, method);
          scene->HandleCollisions();

          accumulatedTime -= timeStep;
          elapsedTime += timeStep;
        }
      } else {
        scene->HandlePhysicsUpdates(dt.asSeconds(), method);
        scene->HandleCollisions();

        elapsedTime += dt.asSeconds();
      }
    }
  }
};
