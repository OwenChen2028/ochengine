#include "Scenes&Rendering.h"
#include <iostream>

Scene* SetupSimulationDemo() {
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

    return scene;
}

Scene* SetupPongDemo() {
    std::vector<Object*> objects;

    objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 25.0f, 200.0f, 50.0f, 400.0f)); // left paddle
    objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 750.0f, 200.0f, 775.0f, 400.0f)); // right paddle

    objects.push_back(new Circle(1.0f, 1.0f, 500.0f, 100.0f, 0.0f, 25.0f, 400.0f, 300.0f)); // ball

    objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -50.0f, 800.0f, 0.0f)); // top boundary
    objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 600.0f, 800.0f, 650.0f)); // bottom boundary
    // objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -50.0f, 0.0f, 0.0f, 600.0f)); // left boundary
    // objects.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 800.0f, 0.0f, 850.0f, 600.0f)); // right boundary

    struct CustomScene : Scene {
        bool keyPressed[4];
        float paddleSpeed;

        CustomScene(std::vector<Object*> objects_) : Scene(objects_) {
            for (int i = 0; i < 4; i++) {
                keyPressed[i] = false;
            }
            paddleSpeed = 500.0f;
        }

        void ProcessEvent(sf::Event event) override {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) { // left paddle controls
                    keyPressed[0] = true;
                }
                else if (event.key.code == sf::Keyboard::S) {
                    keyPressed[1] = true;
                }

                if (event.key.code == sf::Keyboard::Up) { // right paddle controls
                    keyPressed[2] = true;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    keyPressed[3] = true;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) { // left paddle controls
                    keyPressed[0] = false;
                }
                else if (event.key.code == sf::Keyboard::S) {
                    keyPressed[1] = false;
                }

                if (event.key.code == sf::Keyboard::Up) { // right paddle controls
                    keyPressed[2] = false;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    keyPressed[3] = false;
                }
            }
        }

        void HandleUpdates() override {
            Object* leftPaddle = objects[0];
            Object* rightPaddle = objects[1];

            if (keyPressed[0]) {
                leftPaddle->velocityY = -1 * paddleSpeed;
            }
            else if (keyPressed[1]) {
                leftPaddle->velocityY = paddleSpeed;
            }
            else { // no key pressed
                leftPaddle->velocityY = 0.0f;
            }

            if (keyPressed[2]) {
                rightPaddle->velocityY = -1 * paddleSpeed;
            }
            else if (keyPressed[3]) {
                rightPaddle->velocityY = paddleSpeed;
            }
            else { // no key pressed
                rightPaddle->velocityY = 0.0f;
            }

            if (leftPaddle->GetCenterY() <= 100.0f && leftPaddle->velocityY <= 0.0f) {
                leftPaddle->velocityY = 0.0f;
            }
            else if (leftPaddle->GetCenterY() >= 500.0f && leftPaddle->velocityY >= 0.0f) {
                leftPaddle->velocityY = 0.0f;
            }

            if (rightPaddle->GetCenterY() <= 100.0f && rightPaddle->velocityY <= 0.0f) {
                rightPaddle->velocityY = 0.0f;
            }
            else if (rightPaddle->GetCenterY() >= 500.0f && rightPaddle->velocityY >= 0.0f) {
                rightPaddle->velocityY = 0.0f;
            }
        }
    };

    return new CustomScene(objects);
}

void DeleteScene(Scene* scene) {
    for (int i = 0; i < scene->objects.size(); i++) {
        delete scene->objects[i];
    }
    delete scene;
}

int main() {
    Game* game = new Game("Engine Demo", 800, 600, 60, std::vector<Scene*>());

    game->scenes.push_back(SetupSimulationDemo());
    game->scenes.push_back(SetupPongDemo());

    int input = 0;

    while (true) {
        std::cout << "Enter 0 for Simulation Demo, 1 for Pong Demo, 2 to refresh scenes (-1 to exit): ";
        std::cin >> input;

        if (input == -1) {
            std::cout << "Exiting...\n";
            break;
        }
        else if (input == 0 || input == 1) {
            game->OpenWindow();
            game->PlayScene(input, "rk4", true, 0.0f);
            game->CloseWindow();
        }
        else if (input == 2) {
            DeleteScene(game->scenes[0]);
            DeleteScene(game->scenes[1]);

            game->scenes[0] = SetupSimulationDemo();
            game->scenes[1] = SetupPongDemo();
        }
        else {
            std::cout << "Invalid input, try again.\n";
            continue;
        }
    }

    DeleteScene(game->scenes[0]);
    DeleteScene(game->scenes[1]);

    delete game;

    return 0;
}