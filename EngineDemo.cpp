#include "Scenes&Rendering.h"

#include <iostream>

int main() {
    int mode = 0;

    std::cout << "Enter 1 for Simulation Demo, 2 for Pong Demo: ";
    std::cin >> mode;

    if (mode == 1) {
        Scene* scene = new Scene(std::vector<Object*>());

        scene->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 25.0f, 100.0f, 100.0f));
        scene->objects.push_back(new Circle(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 25.0f, 100.0f, 100.0f));

        scene->objects.push_back(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 250.0f, 250.0f, 350.0f, 300.0f));
        scene->objects.push_back(new Rect(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 250.0f, 250.0f, 350.0f, 300.0f));

        scene->objects.push_back(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 350.0f, 350.0f, 450.0f, 450.0f));
        scene->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 50.0f, 400.0f, 400.0f));

        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f, 10.0f)); // top boundary
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 600.0f)); // left boundary
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary

        Display* display = new Display("Simulation Demo", 800, 600, 60, scene);

        display->PlayScene(true, 2);
    }
    else if (mode == 2) {
        Scene* scene = new Scene(std::vector<Object*>());
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 50.0f, 200.0f, 100.0f, 400.0f)); // left paddle
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 700.0f, 200.0f, 750.0f, 400.0f)); // right paddle

        scene->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 0.0f, 25.0f, 100.0f, 100.0f)); // ball

        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f, 10.0f)); // top boundary
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 600.0f)); // left boundary
        scene->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary

        Display* display = new Display("Pong Demo", 800, 600, 60, scene);

        display->PlayScene(true, 2);
    }

    return 0;
}