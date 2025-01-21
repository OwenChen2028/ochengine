#include "Scenes&Rendering.h"
#include <iostream>

Scene* SetupSimDemo() {
    Scene* scene = new Scene();

    scene->objects.addValue(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 25.0f, 100.0f, 100.0f));
    scene->objects.addValue(new Circle(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 25.0f, 100.0f, 100.0f));

    scene->objects.addValue(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 250.0f, 250.0f, 350.0f, 300.0f));
    scene->objects.addValue(new Rect(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 250.0f, 250.0f, 350.0f, 300.0f));

    scene->objects.addValue(new Rect(1.0f, 1.0f, -100.0f, 0.0f, 1000.0f, 350.0f, 350.0f, 450.0f, 450.0f));
    scene->objects.addValue(new Circle(1.0f, 1.0f, 100.0f, 0.0f, 1000.0f, 50.0f, 400.0f, 400.0f));

    scene->objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f, 10.0f)); // top boundary
    scene->objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
    scene->objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 600.0f)); // left boundary
    scene->objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary

    return scene;
}

struct PongScene : Scene {
    bool keyPressed[4];
    float paddleSpeed;

    PongScene(Container<Object*> objects_) : Scene(objects_) {
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
        Object* leftPaddle = objects.getValue(0);
        Object* rightPaddle = objects.getValue(1);

        leftPaddle->velocityY = 0.0f; // left paddle movment

        if (keyPressed[0]) {
            leftPaddle->velocityY -= paddleSpeed;
        }

        if (keyPressed[1]) {
            leftPaddle->velocityY += paddleSpeed;
        }

        rightPaddle->velocityY = 0.0f; // right paddle movement

        if (keyPressed[2]) {
            rightPaddle->velocityY -= paddleSpeed;
        }

        if (keyPressed[3]) {
            rightPaddle->velocityY += paddleSpeed;
        }

        if (leftPaddle->GetCenterY() <= 100.0f && leftPaddle->velocityY <= 0.0f) { // clamp left paddle movement
            leftPaddle->velocityY = 0.0f;
        }
        else if (leftPaddle->GetCenterY() >= 500.0f && leftPaddle->velocityY >= 0.0f) {
            leftPaddle->velocityY = 0.0f;
        }

        if (rightPaddle->GetCenterY() <= 100.0f && rightPaddle->velocityY <= 0.0f) { // clamp right paddle movement
            rightPaddle->velocityY = 0.0f;
        }
        else if (rightPaddle->GetCenterY() >= 500.0f && rightPaddle->velocityY >= 0.0f) {
            rightPaddle->velocityY = 0.0f;
        }
    }
};

Scene* SetupPongDemo() {
    Container<Object*> objects;

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 25.0f, 200.0f, 50.0f, 400.0f)); // left paddle
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 750.0f, 200.0f, 775.0f, 400.0f)); // right paddle

    objects.addValue(new Circle(1.0f, 1.0f, 500.0f, 100.0f, 0.0f, 25.0f, 400.0f, 300.0f)); // ball

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -50.0f, 800.0f, 0.0f)); // top boundary
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 600.0f, 800.0f, 650.0f)); // bottom boundary

    // objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -50.0f, 0.0f, 0.0f, 600.0f)); // left boundary
    // objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 800.0f, 0.0f, 850.0f, 600.0f)); // right boundary

    return new PongScene(objects);
}

int main() {
    Game* game = new Game("Engine Demo", 800, 600);

    game->scenes.addValue(SetupSimDemo());
    game->scenes.addValue(SetupPongDemo());

    int input = 0;

    std::cout << "Commands:\n0 - simulation demo\n1 - pong demo\n2 - reload scenes\n-1 - exit program\n\nInputs:\n";

    while (std::cin >> input) {
        if (input == -1) {
            std::cout << "Exiting...\n";
            break;
        }
        else if (input == 0 || input == 1) { // input is sceneId
            game->ToggleWindow(true);
            if (input == 0) {
                game->PlayScene(input);
            }
            else if (input == 1) {
                game->PlayScene(input, 0.0f, "euler", false, true);
            }
            game->ToggleWindow(false);
        }
        else if (input == 2) {
            delete game->scenes.getValue(0);
            delete game->scenes.getValue(1);

            game->scenes.setValue(SetupSimDemo(), 0);
            game->scenes.setValue(SetupPongDemo(), 1);
        }
        else {
            std::cout << "Unknown command.\n";
            continue;
        }
    }

    delete game;

    return 0;
}
