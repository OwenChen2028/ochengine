#include "ScenesNRendering.h"

#include <iostream>

int main() {
    Scene* scene1 = new Scene(60, true);

    scene1->objects.push_back(new Circle(1.0f, 1.0f, 200.0f, 0.0f, 50000.0f, 25, 100.0f, 100.0f));
    scene1->objects.push_back(new Circle(1.0f, 1.0f, -200.0f, 0.0f, 50000.0f, 25.0f, 100.0f, 100.0f));

    scene1->objects.push_back(new Rect(1.0f, 1.0f, -400.0f, 0.0f, 50000.0f, 250.0f, 250.0f, 350.0f, 300.0f));
    scene1->objects.push_back(new Rect(1.0f, 1.0f, 400.0f, 0.0f, 50000.0f, 250.0f, 250.0f, 350.0f, 300.0f));

    scene1->objects.push_back(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 50000.0f, 350.0f, 350.0f, 450.0f, 450.0f));
    scene1->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 50000.0f, 50.0f, 400.0f, 400.0f));

    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f, 10.0f)); // top boundary
    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 600.0f)); // left boundary
    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary

    scene1->SimulatePhysics(0.0f, true);

    return 0;
}