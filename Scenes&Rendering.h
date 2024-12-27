#include "Objects&Collisions.h"

#include <vector>
#include <SFML/Graphics.hpp>

struct Scene {
	std::vector<Object*> objects; // todo: broad phase collision detection w/ diff data structure

	Scene(std::vector<Object*> objects_) {
		objects = objects_;
	}

	void HandleUpdates(float dt, int method) {
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->Update(dt, method);
		}
	}

	void HandleCollisions() {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				Collision* col = NULL;
				bool collision = false;

				if (objects[i]->shape == 1) {
					if (objects[j]->shape == 1) { // rect and rect
						col = new Collision(objects[i], objects[j]);

						if (CheckRectRectCol(col)) {
							collision = true;
						}
					}
					else if (objects[j]->shape == 2) { // rect and circle
						col = new Collision(objects[i], objects[j]);

						if (CheckRectCircleCol(col)) {
							collision = true;
						}
					}
				}
				else if (objects[i]->shape == 2) {
					if (objects[j]->shape == 1) { // circle and rect
						col = new Collision(objects[j], objects[i]); // swap to rect and circle

						if (CheckRectCircleCol(col)) {
							collision = true;
						}
					}
					else if (objects[j]->shape == 2) { // circle and circle
						col = new Collision(objects[i], objects[j]);

						if (CheckCircleCircleCol(col)) {
							collision = true;
						}
					}
				}

				if (collision) {
					ResolveCollision(col);
					CorrectPositions(col);
				}

				if (col != NULL) {
					delete col;
				}
			}
		}
	}
};

struct Display {
	sf::RenderWindow window;

	int windowWidth;
	int windowHeight;

	int updateFreq;

	Scene* scene;

	Display(const char* windowName, int windowWidth_, int windowHeight_, int updateFreq_, Scene* scene_) {
		windowWidth = windowWidth_;
		windowHeight = windowHeight_;

		updateFreq = updateFreq_;

		scene = scene_;

		window.create(sf::VideoMode(windowWidth, windowHeight), windowName);
		window.setFramerateLimit(updateFreq);
	}

	void DrawObjects() {
		if (!window.isOpen()) {
			return;
		}

		window.clear();

		sf::RectangleShape rectShape;
		sf::CircleShape circleShape;

		for (int i = 0; i < scene->objects.size(); i++) {
			if (scene->objects[i]->shape == 1) { // rect
				Rect* rect = static_cast<Rect*>(scene->objects[i]);

				rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
				rectShape.setPosition(rect->minX, rect->minY);
				rectShape.setFillColor(sf::Color::White);

				window.draw(rectShape);
			}
			else if (scene->objects[i]->shape == 2) { // circle
				Circle* circle = static_cast<Circle*>(scene->objects[i]);

				circleShape.setRadius(circle->radius);
				circleShape.setPosition(circle->posX - circle->radius, circle->posY - circle->radius);
				circleShape.setFillColor(sf::Color::White);

				window.draw(circleShape);
			}
		}

		window.display();
	}

	void PlayScene(bool deterministic, int method) {
		sf::Clock clock;

		float elapsedTime = 0.0f;

		float accumulatedTime = 0.0f;
		float timeStep = 1.0f / updateFreq;

		while (window.isOpen()) {
			sf::Time dt = clock.restart();

			DrawObjects();

			if (deterministic) {
				accumulatedTime += dt.asSeconds();

				while (accumulatedTime >= timeStep) {
					scene->HandleUpdates(timeStep, method);
					scene->HandleCollisions();

					accumulatedTime -= timeStep;
					elapsedTime += timeStep;
				}
			}
			else {
				scene->HandleUpdates(dt.asSeconds(), method);
				scene->HandleCollisions();

				elapsedTime += dt.asSeconds();
			}

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
			}
		}
	}
};