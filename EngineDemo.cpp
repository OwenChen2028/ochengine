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
    std::vector<Object*> objects2;

    objects2.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 25.0f, 200.0f, 50.0f, 400.0f)); // left paddle
    objects2.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 750.0f, 200.0f, 775.0f, 400.0f)); // right paddle

    objects2.push_back(new Circle(1.0f, 1.0f, 500.0f, 250.0f, 0.0f, 25.0f, 400.0f, 300.0f)); // ball

    objects2.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -50.0f, 800.0f, 0.0f)); // top boundary
    objects2.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 600.0f, 800.0f, 650.0f)); // bottom boundary
    // objects2.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -50.0f, 0.0f, 0.0f, 600.0f)); // left boundary
    // objects2.push_back(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 800.0f, 0.0f, 850.0f, 600.0f)); // right boundary

    struct Scene2 : Scene {
        bool keyPressed[4];

        float paddleSpeed;

        Scene2(std::vector<Object*> objects_) : Scene(objects_) {
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

    Scene* scene2 = new Scene2(objects2);

    // run scenes
    Game* game = new Game("Simulation Demo", 800, 600, 60, std::vector<Scene*>());

    game->scenes.push_back(scene1);
    game->scenes.push_back(scene2);

    int sceneId = 0;

    while (true) {
        std::cout << "Enter 0 for Simulation Demo, 1 for Pong Demo (-1 to exit): ";
        std::cin >> sceneId;

        if (sceneId == -1) {
            std::cout << "Exiting...\n";
            break;
        }
        else if (sceneId >= game->scenes.size()) {
            std::cout << "Invalid input, try again.\n";
            continue;
        }

        game->OpenWindow();
        game->PlayScene(sceneId, "rk4", true, 0.0f);
        game->CloseWindow();
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