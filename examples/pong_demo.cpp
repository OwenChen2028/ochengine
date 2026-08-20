#include "ochengine/game.hpp"

namespace {

struct PongScene : Scene {
  bool keyPressed[4];
  bool waitingForServe;
  bool gameOver;
  bool paddleColliding;
  bool paddleCollidingLastUpdate;

  int leftScore;
  int rightScore;

  float paddleSpeed;
  float serveDirection;
  float verticalServeDirection;

  PongScene() {
    paddleSpeed = 500.0f;
    Reset();
  }

  void HandleEvent(sf::Event event) override {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::R) {
        Reset();
      } else if (event.key.code == sf::Keyboard::Space && waitingForServe && !gameOver) {
        LaunchBall();
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
    } else if (event.type == sf::Event::LostFocus) {
      for (int i = 0; i < 4; i++) {
        keyPressed[i] = false;
      }
    }
  }

  void HandleFixedUpdate(float) override {
    Object *leftPaddle = objects.getValue(0);
    Object *rightPaddle = objects.getValue(1);
    Circle *ball = static_cast<Circle *>(objects.getValue(2));

    paddleCollidingLastUpdate = paddleColliding;
    paddleColliding = false;

    if (!waitingForServe && !gameOver && ball->posX + ball->radius < 0.0f) {
      rightScore++;
      PrepareServe(-1.0f);
    } else if (!waitingForServe && !gameOver && ball->posX - ball->radius > 800.0f) {
      leftScore++;
      PrepareServe(1.0f);
    }

    if (leftScore == 11 || rightScore == 11) {
      waitingForServe = false;
      gameOver = true;
    }

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

  void OnCollisionStay(Collision &col) override {
    Circle *ball = static_cast<Circle *>(objects.getValue(2));
    Object *other = nullptr;

    if (col.object1 == ball) {
      other = col.object2;
    } else if (col.object2 == ball) {
      other = col.object1;
    }

    if (other != objects.getValue(0) && other != objects.getValue(1)) {
      return;
    }

    if (!paddleCollidingLastUpdate && !paddleColliding) {
      float speed = FindSqrt(FindSquare(ball->velocityX) + FindSquare(ball->velocityY));

      if (speed > 0.0f && speed < 700.0f) {
        float newSpeed = FindMin(speed * 1.05f, 700.0f);
        ball->velocityX *= newSpeed / speed;
        ball->velocityY *= newSpeed / speed;
      }
    }

    paddleColliding = true;
  }

  void HandleDraw(sf::RenderWindow &window) override {
    UpdateTitle(window);
    Scene::HandleDraw(window);
  }

private:
  void LaunchBall() {
    Circle *ball = static_cast<Circle *>(objects.getValue(2));

    waitingForServe = false;
    ball->velocityX = serveDirection * 500.0f;
    ball->velocityY = verticalServeDirection * 100.0f;
    verticalServeDirection *= -1.0f;
  }

  void PrepareServe(float direction) {
    Circle *ball = static_cast<Circle *>(objects.getValue(2));

    waitingForServe = true;
    serveDirection = direction;
    ball->posX = 400.0f;
    ball->posY = 300.0f;
    ball->velocityX = 0.0f;
    ball->velocityY = 0.0f;
    paddleColliding = false;
    paddleCollidingLastUpdate = false;
  }

  void Reset() {
    for (int i = 0; i < objects.getSize(); i++) {
      delete objects.getValue(i);
    }
    objects = Container<Object *>();

    for (int i = 0; i < 4; i++) {
      keyPressed[i] = false;
    }

    leftScore = 0;
    rightScore = 0;
    gameOver = false;
    verticalServeDirection = 1.0f;

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 25.0f, 200.0f, 50.0f, 400.0f));   // left paddle
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 750.0f, 200.0f, 775.0f, 400.0f)); // right paddle

    objects.addValue(new Circle(1.0f, 1.0f, 500.0f, 100.0f, 0.0f, 25.0f, 400.0f, 300.0f)); // ball

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -50.0f, 800.0f, 0.0f));   // top boundary
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 600.0f, 800.0f, 650.0f)); // bottom boundary

    PrepareServe(1.0f);
  }

  void UpdateTitle(sf::RenderWindow &window) {
    sf::String title = "Pong | ";
    title += ScoreString(leftScore);
    title += " - ";
    title += ScoreString(rightScore);

    if (gameOver) {
      if (leftScore == 11) {
        title += " | LEFT WINS - R to restart";
      } else {
        title += " | RIGHT WINS - R to restart";
      }
    } else if (waitingForServe) {
      title += " | SPACE to serve";
    }

    window.setTitle(title);
  }

  sf::String ScoreString(int score) {
    char string[3];

    if (score >= 10) {
      string[0] = '0' + score / 10;
      string[1] = '0' + score % 10;
      string[2] = '\0';
    } else {
      string[0] = '0' + score;
      string[1] = '\0';
    }

    return string;
  }
};

} // namespace

int main() {
  Game game("Pong Demo", 800, 600);
  game.scenes.addValue(new PongScene());

  game.ToggleWindow(true);
  game.window.setKeyRepeatEnabled(false);
  game.PlayScene(0, 0.0f, IntegrationType::Euler, false, true);

  return 0;
}
