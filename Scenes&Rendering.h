#include "Objects&Collisions.h"

#include <vector>
#include <SFML/Graphics.hpp>

struct Scene {
	std::vector<Object*> objects; // todo: use diff data structure (e.g. quadtree)

	Scene(std::vector<Object*> objects_) {
		objects = objects_;
	}

	virtual ~Scene() {
		for (Object* obj : objects) {
			delete obj;
		}
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

	~Game() {
		for (Scene* scene : scenes) {
			delete scene;
		}
	}

	void SetWindowActive(bool active) {
		if (active) {
			window.create(sf::VideoMode(windowWidth, windowHeight), windowName);
			window.setFramerateLimit(updateFreq);
		}
		else {
			window.close();
		}
	}

	void DrawObjects(Scene* scene) {
		if (!window.isOpen()) {
			return;
		}

		window.clear();

		sf::RectangleShape rectShape;
		sf::CircleShape circleShape;

		for (int i = 0; i < scene->objects.size(); i++) {
			if (scene->objects[i]->shape == "rect") {
				Rect* rect = static_cast<Rect*>(scene->objects[i]);

				rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
				rectShape.setPosition(rect->minX, rect->minY);
				rectShape.setFillColor(sf::Color::White);

				window.draw(rectShape);
			}
			else if (scene->objects[i]->shape == "circle") {
				Circle* circle = static_cast<Circle*>(scene->objects[i]);

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

		Scene* scene = scenes[sceneId];

		while (elapsedTime < duration || duration == 0.0f) { // 0 for infinite duration
			sf::Time dt = clock.restart();

			DrawObjects(scene);

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					SetWindowActive(false);
					break;
				}
				else {
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
			}
			else {
				scene->HandlePhysicsUpdates(dt.asSeconds(), method);
				scene->HandleCollisions();

				elapsedTime += dt.asSeconds();
			}
		}
	}
};