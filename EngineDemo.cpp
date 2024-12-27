#include "Scenes&Rendering.h"

#include <iostream>

int main() {
    // simulation
    Scene* scene1 = new Scene(std::vector<Object*>());

    scene1->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 25.0f, 100.0f, 100.0f));
    scene1->objects.push_back(new Circle(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 25.0f, 100.0f, 100.0f));

    scene1->objects.push_back(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 250.0f, 250.0f, 350.0f, 300.0f));
    scene1->objects.push_back(new Rect(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 250.0f, 250.0f, 350.0f, 300.0f));

    scene1->objects.push_back(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 350.0f, 350.0f, 450.0f, 450.0f));
    scene1->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 50.0f, 400.0f, 400.0f));

    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f, 10.0f)); // top boundary
    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 600.0f)); // left boundary
    scene1->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary
    
    // pong
    Scene* scene2 = new Scene(std::vector<Object*>());

    scene2->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 50.0f, 200.0f, 100.0f, 400.0f)); // left paddle
    scene2->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 700.0f, 200.0f, 750.0f, 400.0f)); // right paddle

    scene2->objects.push_back(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 0.0f, 25.0f, 400.0f, 300.0f)); // ball

    scene2->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f, 10.0f)); // top boundary
    scene2->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
    scene2->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 600.0f)); // left boundary
    scene2->objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary

    // run scenes
    Game* game = new Game("Simulation Demo", 800, 600, 60, std::vector<Scene*>());

    game->scenes.push_back(scene1);
    game->scenes.push_back(scene2);

    int sceneIndex = 0;

    while (true) {
        std::cout << "Enter 0 for Simulation Demo, 1 for Pong Demo (-1 to exit): ";
        std::cin >> sceneIndex;

        if (sceneIndex == -1) {
            std::cout << "Exiting...\n";
            break;
        }
        else if (sceneIndex >= game->scenes.size()) {
            std::cout << "Invalid input, try again.\n";
            continue;
        }

        game->OpenWindow();
        game->PlayScene(sceneIndex, "rk4", true);
    }

    for (int i = 0; i < scene1->objects.size(); i++) {
        delete scene1->objects[i];
    }

    for (int i = 0; i < scene2->objects.size(); i++) {
        delete scene2->objects[i];
    }

    delete scene1;
    delete scene2;

    delete game;

    return 0;
}