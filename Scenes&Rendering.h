#include "Objects&Collisions.h"

#include <vector>
#include <SFML/Graphics.hpp>

struct Scene {
	std::vector<Object*> objects; // todo: broad phase collision detection w/ diff data structure

	Scene(std::vector<Object*> objects_) {
		objects = objects_;
	}

	void HandleUpdates(float dt, const char* method) {
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->Update(dt, method);
		}
	}

	void HandleCollisions() {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				Collision* col = NULL;
				bool collision = false;

				if (objects[i]->shape == "rect") {
					if (objects[j]->shape == "rect") {
						col = new Collision(objects[i], objects[j]);

						if (CheckRectRectCol(col)) {
							collision = true;
						}
					}
					else if (objects[j]->shape == "circle") {
						col = new Collision(objects[i], objects[j]);

						if (CheckRectCircleCol(col)) {
							collision = true;
						}
					}
				}
				else if (objects[i]->shape == "circle") {
					if (objects[j]->shape == "rect") {
						col = new Collision(objects[j], objects[i]); // swap to rect and circle

						if (CheckRectCircleCol(col)) {
							collision = true;
						}
					}
					else if (objects[j]->shape == "circle") {
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

struct Game {
	sf::RenderWindow window;

	const char* windowName;

	int windowWidth;
	int windowHeight;

	int updateFreq;

	std::vector<Scene*> scenes;

	Game(const char* windowName_, int windowWidth_, int windowHeight_, int updateFreq_, std::vector<Scene*> scenes_) {
		windowName = windowName_;
		
		windowWidth = windowWidth_;
		windowHeight = windowHeight_;

		updateFreq = updateFreq_;

		scenes = scenes_;
	}

	void OpenWindow() {
		window.create(sf::VideoMode(windowWidth, windowHeight), windowName);
		window.setFramerateLimit(updateFreq);
	}

	void DrawObjects(int sceneIndex) {
		if (!window.isOpen()) {
			return;
		}

		window.clear();

		sf::RectangleShape rectShape;
		sf::CircleShape circleShape;

		for (int i = 0; i < scenes[sceneIndex]->objects.size(); i++) {
			if (scenes[sceneIndex]->objects[i]->shape == "rect") {
				Rect* rect = static_cast<Rect*>(scenes[sceneIndex]->objects[i]);

				rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
				rectShape.setPosition(rect->minX, rect->minY);
				rectShape.setFillColor(sf::Color::White);

				window.draw(rectShape);
			}
			else if (scenes[sceneIndex]->objects[i]->shape == "circle") {
				Circle* circle = static_cast<Circle*>(scenes[sceneIndex]->objects[i]);

				circleShape.setRadius(circle->radius);
				circleShape.setPosition(circle->posX - circle->radius, circle->posY - circle->radius);
				circleShape.setFillColor(sf::Color::White);

				window.draw(circleShape);
			}
		}

		window.display();
	}

	void PlayScene(int sceneIndex, const char* method, bool deterministic) {
		sf::Clock clock;

		float elapsedTime = 0.0f;

		float accumulatedTime = 0.0f;
		float timeStep = 1.0f / updateFreq;

		while (window.isOpen()) {
			sf::Time dt = clock.restart();

			DrawObjects(sceneIndex);

			if (deterministic) {
				accumulatedTime += dt.asSeconds();

				while (accumulatedTime >= timeStep) {
					scenes[sceneIndex]->HandleUpdates(timeStep, method);
					scenes[sceneIndex]->HandleCollisions();

					accumulatedTime -= timeStep;
					elapsedTime += timeStep;
				}
			}
			else {
				scenes[sceneIndex]->HandleUpdates(dt.asSeconds(), method);
				scenes[sceneIndex]->HandleCollisions();

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