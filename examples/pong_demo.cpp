#include "ochengine/game.hpp"

namespace {

struct PongScene : Scene {
  bool keyPressed[4];
  float paddleSpeed;

  PongScene() {
    paddleSpeed = 500.0f;
    Reset();
  }

  void HandleEvent(sf::Event event) override {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::R) {
        Reset();
      } else if (event.key.code == sf::Keyboard::W) { // left paddle controls
        keyPressed[0] = true;
      } else if (event.key.code == sf::Keyboard::S) {
        keyPressed[1] = true;
      }

      if (event.key.code == sf::Keyboard::Up) { // right paddle controls
        keyPressed[2] = true;
      } else if (event.key.code == sf::Keyboard::Down) {
        keyPressed[3] = true;
      }
    } else if (event.type == sf::Event::KeyReleased) {
      if (event.key.code == sf::Keyboard::W) { // left paddle controls
        keyPressed[0] = false;
      } else if (event.key.code == sf::Keyboard::S) {
        keyPressed[1] = false;
      }

      if (event.key.code == sf::Keyboard::Up) { // right paddle controls
        keyPressed[2] = false;
      } else if (event.key.code == sf::Keyboard::Down) {
        keyPressed[3] = false;
      }
    }
  }

  void HandleFixedUpdate(float dt) override {
    Object *leftPaddle = objects.getValue(0);
    Object *rightPaddle = objects.getValue(1);

    leftPaddle->velocityY = 0.0f; // left paddle movement

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
    } else if (leftPaddle->GetCenterY() >= 500.0f && leftPaddle->velocityY >= 0.0f) {
      leftPaddle->velocityY = 0.0f;
    }

    if (rightPaddle->GetCenterY() <= 100.0f && rightPaddle->velocityY <= 0.0f) { // clamp right paddle movement
      rightPaddle->velocityY = 0.0f;
    } else if (rightPaddle->GetCenterY() >= 500.0f && rightPaddle->velocityY >= 0.0f) {
      rightPaddle->velocityY = 0.0f;
    }
  }

private:
  void Reset() {
    for (int i = 0; i < objects.getSize(); i++) {
      delete objects.getValue(i);
    }
    objects = Container<Object *>();

    for (int i = 0; i < 4; i++) {
      keyPressed[i] = false;
    }

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 25.0f, 200.0f, 50.0f, 400.0f));   // left paddle
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 750.0f, 200.0f, 775.0f, 400.0f)); // right paddle

    objects.addValue(new Circle(1.0f, 1.0f, 500.0f, 100.0f, 0.0f, 25.0f, 400.0f,
                                300.0f)); // ball

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -50.0f, 800.0f, 0.0f));   // top boundary
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 600.0f, 800.0f, 650.0f)); // bottom boundary
  }
};

} // namespace

int main() {
  Game game("Pong Demo", 800, 600);
  game.scenes.addValue(new PongScene());

  game.ToggleWindow(true);
  game.PlayScene(0, 0.0f, IntegrationType::Euler, false, true);

  return 0;
}
