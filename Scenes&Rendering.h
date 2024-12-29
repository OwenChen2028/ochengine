#include "Objects&Collisions.h"

#include <vector>
#include <SFML/Graphics.hpp>

struct Scene {
	std::vector<Object*> objects; // todo: use diff data structure (e.g. quadtree)

	Scene(std::vector<Object*> objects_) {
		objects = objects_;
	}

	void HandlePhysicsUpdates(float dt, const char* method) {
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->PhysicsUpdate(dt, method);
		}
	}

	void HandleCollisions() {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				Collision* col = nullptr;
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

				if (col != nullptr) {
					if (collision) {
						ResolveCollision(col);
						CorrectPositions(col);
					}

					delete col;
				}
			}
		}
	}

	virtual void ProcessEvent(sf::Event event) { }
	virtual void HandleUpdates() { }
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
		if (!window.isOpen()) {
			window.create(sf::VideoMode(windowWidth, windowHeight), windowName);
			window.setFramerateLimit(updateFreq);
		}
	}

	void CloseWindow() {
		if (window.isOpen()) {
			window.close();
		}
	}

	void DrawObjects(int sceneId) {
		if (!window.isOpen()) {
			return;
		}

		window.clear();

		sf::RectangleShape rectShape;
		sf::CircleShape circleShape;

		for (int i = 0; i < scenes[sceneId]->objects.size(); i++) {
			if (scenes[sceneId]->objects[i]->shape == "rect") {
				Rect* rect = static_cast<Rect*>(scenes[sceneId]->objects[i]);

				rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
				rectShape.setPosition(rect->minX, rect->minY);
				rectShape.setFillColor(sf::Color::White);

				window.draw(rectShape);
			}
			else if (scenes[sceneId]->objects[i]->shape == "circle") {
				Circle* circle = static_cast<Circle*>(scenes[sceneId]->objects[i]);

				circleShape.setRadius(circle->radius);
				circleShape.setPosition(circle->posX - circle->radius, circle->posY - circle->radius);
				circleShape.setFillColor(sf::Color::White);

				window.draw(circleShape);
			}
		}

		window.display();
	}

	void PlayScene(int sceneId, const char* method, bool deterministic, float duration) {
		sf::Clock clock;

		float elapsedTime = 0.0f;

		float accumulatedTime = 0.0f;
		float timeStep = 1.0f / updateFreq;

		while (elapsedTime < duration || duration == 0.0f) { // 0 for infinite duration
			sf::Time dt = clock.restart();

			DrawObjects(sceneId);

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
					return;
				}
				else {
					scenes[sceneId]->ProcessEvent(event);
				}
			}

			scenes[sceneId]->HandleUpdates();

			if (deterministic) {
				accumulatedTime += dt.asSeconds();

				while (accumulatedTime >= timeStep) {
					scenes[sceneId]->HandlePhysicsUpdates(timeStep, method);
					scenes[sceneId]->HandleCollisions();

					accumulatedTime -= timeStep;
					elapsedTime += timeStep;
				}
			}
			else {
				scenes[sceneId]->HandlePhysicsUpdates(dt.asSeconds(), method);
				scenes[sceneId]->HandleCollisions();

				elapsedTime += dt.asSeconds();
			}
		}
	}
};