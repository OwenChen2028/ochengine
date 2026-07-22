#pragma once

#include "ochengine/collision.hpp"
#include "ochengine/container.hpp"
#include <SFML/Window/Event.hpp>

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
