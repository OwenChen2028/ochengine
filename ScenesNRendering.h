#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "ObjectsNCollisions.h"

struct Scene {
	std::vector<Object*> objects;

	int updateFreq;

	bool rendering;
	sf::RenderWindow window;
	
	Scene(int updateFreq_, bool rendering_) {
		updateFreq = updateFreq_;

		rendering = rendering_;

		if (rendering) {
			window.create(sf::VideoMode(800, 600), "Physics Simulation");
			window.setFramerateLimit(updateFreq);
		}
	}

	void HandleUpdates(float dt) {
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->Update(dt);
		}
	}

	void HandleCollisions() {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				if (objects[i]->shape == 1) {
					if (objects[j]->shape == 1) { // rect and rect
						Collision* col = new Collision(objects[i], objects[j]);

						if (CheckRectRectCol(col)) {
							ResolveCollision(col);
							CorrectPositions(col);
						}

						delete col;
					}
					else if (objects[j]->shape == 2) { // rect and circle
						Collision* col = new Collision(objects[i], objects[j]);

						if (CheckRectCircleCol(col)) {
							ResolveCollision(col);
							CorrectPositions(col);
						}

						delete col;
					}
				}
				else if (objects[i]->shape == 2) {
					if (objects[j]->shape == 1) { // circle and rect
						Collision* col = new Collision(objects[j], objects[i]); // swap to rect and circle

						if (CheckRectCircleCol(col)) {
							ResolveCollision(col);
							CorrectPositions(col);
						}

						delete col;
					}
					else if (objects[j]->shape == 2) { // circle and circle
						Collision* col = new Collision(objects[i], objects[j]);

						if (CheckCircleCircleCol(col)) {
							ResolveCollision(col);
							CorrectPositions(col);
						}

						delete col;
					}
				}
			}
		}
	}

	void HandleRendering() {
		if (!rendering || !window.isOpen()) {
			return;
		}

		window.clear();

		sf::RectangleShape rectShape;
		sf::CircleShape circleShape;

		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->shape == 1) { // rect
				Rect* rect = static_cast<Rect*>(objects[i]);

				rectShape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
				rectShape.setPosition(rect->minX, rect->minY);
				rectShape.setFillColor(sf::Color::White);

				window.draw(rectShape);
			}
			else if (objects[i]->shape == 2) { // circle
				Circle* circle = static_cast<Circle*>(objects[i]);

				circleShape.setRadius(circle->radius);
				circleShape.setPosition(circle->posX - circle->radius, circle->posY - circle->radius);
				circleShape.setFillColor(sf::Color::White);

				window.draw(circleShape);
			}
		}

		window.display();
	}

	void SimulatePhysics(float duration, bool deterministic) {
		sf::Clock clock;

		float elapsedTime = 0.0f;

		float accumulatedTime = 0.0f;
		float timeStep = 1.0f / updateFreq;

		while (elapsedTime <= duration || duration == 0) { // 0 for indefinite duration
			sf::Time dt = clock.restart();

			if (rendering) {
				HandleRendering();

				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
				}

				if (!window.isOpen()) {
					return;
				}
			}
			
			if (deterministic) {
				accumulatedTime += dt.asSeconds();

				while (accumulatedTime >= timeStep) {
					HandleUpdates(timeStep);
					HandleCollisions();

					accumulatedTime -= timeStep;
					elapsedTime += timeStep;
				}
			}
			else {
				HandleUpdates(dt.asSeconds());
				HandleCollisions();

				elapsedTime += dt.asSeconds();
			}
		}
	}
};