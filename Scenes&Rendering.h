#include "Objects&Collisions.h"

#include <SFML/Graphics.hpp>

template <typename T> struct Container {
	T* values;

	int size;
	int capacity;

	Container() {
		size = 0;
		capacity = 10;

		values = new T[capacity];
	}

	void addValue(T value) {
		if (size == capacity) {
			capacity *= 2;
			T* newValues = new T[capacity];
			
			for (int i = 0; i < size; i++) {
				newValues[i] = values[i];
			}

			delete[] values;
			values = newValues;
		}

		values[size] = value;
		size++;
	}

	void setValue(T value, int index) {
		if (index >= 0 && index < size) {
			values[index] = value;
		}
		else if (index == capacity) {
			addValue(value);
		}
	}

	T getValue(int index) {
		if (index >= 0 && index < size) {
			return values[index];
		}
	}

	int getSize() {
		return size;
	}

	int getCapacity() {
		return capacity;
	}
};

struct Scene {
	Container<Object*> objects; // todo: use diff data structure (e.g. quadtree)

	Scene(Container<Object*> objects_ = Container<Object*>()) {
		objects = objects_;
	}

	virtual ~Scene() {
		for (int i = 0; i < objects.getSize(); i++) {
			delete objects.getValue(i);
		}
	}

	void HandlePhysicsUpdates(float dt, const char* method) {
		for (int i = 0; i < objects.getSize(); i++) {
			objects.getValue(i)->PhysicsUpdate(dt, method);
		}
	}

	void HandleCollisions() {
		for (int i = 0; i < objects.getSize(); i++) {
			for (int j = i + 1; j < objects.getSize(); j++) {
				Collision* col = nullptr;
				bool collision = false;

				if (objects.getValue(i)->shape == "rect") {
					if (objects.getValue(j)->shape == "rect") {
						col = new Collision(objects.getValue(i), objects.getValue(j));

						if (CheckRectRectCol(col)) {
							collision = true;
						}
					}
					else if (objects.getValue(j)->shape == "circle") {
						col = new Collision(objects.getValue(i), objects.getValue(j));

						if (CheckRectCircleCol(col)) {
							collision = true;
						}
					}
				}
				else if (objects.getValue(i)->shape == "circle") {
					if (objects.getValue(j)->shape == "rect") {
						col = new Collision(objects.getValue(j), objects.getValue(i)); // swap to rect and circle

						if (CheckRectCircleCol(col)) {
							collision = true;
						}
					}
					else if (objects.getValue(j)->shape == "circle") {
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

	Container<Scene*> scenes;

	Game(const char* windowName_, int windowWidth_, int windowHeight_, int updateFreq_ = 60, Container<Scene*> scenes_ = Container<Scene*>()) {
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

		for (int i = 0; i < scene->objects.getSize(); i++) {
			if (scene->objects.getValue(i)->shape == "rect") {
				Rect* rect = static_cast<Rect*>(scene->objects.getValue(i));

				rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
				rectShape.setPosition(rect->minX, rect->minY);
				rectShape.setFillColor(sf::Color::White);

				window.draw(rectShape);
			}
			else if (scene->objects.getValue(i)->shape == "circle") {
				Circle* circle = static_cast<Circle*>(scene->objects.getValue(i));

				circleShape.setRadius(circle->radius);
				circleShape.setPosition(circle->posX - circle->radius, circle->posY - circle->radius);
				circleShape.setFillColor(sf::Color::White);

				window.draw(circleShape);
			}
		}

		window.display();
	}

	void PlayScene(int sceneId, float duration = 0.0f, const char* method = "rk4", bool deterministic = true, bool waitForFocus = false) {
		Scene* scene = scenes.getValue(sceneId);

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

		while (elapsedTime < duration || duration == 0.0f) { // 0 for infinite duration
			sf::Time dt = clock.restart();

			DrawObjects(scene);

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					ToggleWindow(false);
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