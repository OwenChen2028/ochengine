#include "ochengine/game.hpp"
#include <SFML/Graphics.hpp>

namespace {

struct Brick {
  Rect *object;
  sf::Color color;
};

struct BrickBreakerScene : Scene {
  Rect *paddle;
  Circle *ball;
  Brick bricks[40];

  bool leftPressed;
  bool rightPressed;
  bool waitingForServe;
  bool gameOver;
  bool levelComplete;

  int score;
  int lives;
  int level;
  int bricksRemaining;

  float serveDirection;

  BrickBreakerScene() {
    paddle = nullptr;
    ball = nullptr;

    for (int i = 0; i < 40; i++) {
      bricks[i].object = nullptr;
    }

    paddle = new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 325.0f, 550.0f, 475.0f, 570.0f);
    objects.addValue(paddle);

    ball = new Circle(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 10.0f, 400.0f, 535.0f);
    objects.addValue(ball);

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -30.0f, 800.0f, 20.0f));
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -30.0f, 0.0f, 0.0f, 600.0f));
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 800.0f, 0.0f, 830.0f, 600.0f));

    ResetGame();
  }

  void HandleEvent(sf::Event event) override {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
        leftPressed = true;
      } else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
        rightPressed = true;
      } else if (event.key.code == sf::Keyboard::Space) {
        if (waitingForServe) {
          LaunchBall();
        }
      } else if (event.key.code == sf::Keyboard::R) {
        ResetGame();
      }
    } else if (event.type == sf::Event::KeyReleased) {
      if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
        leftPressed = false;
      } else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
        rightPressed = false;
      }
    } else if (event.type == sf::Event::LostFocus) {
      leftPressed = false;
      rightPressed = false;
    }
  }

  void HandleFixedUpdate(float dt) override {
    if (levelComplete) {
      level++;
      SetupBricks();
      PrepareServe();
      levelComplete = false;
    } else if (!waitingForServe && !gameOver && ball->posY - ball->radius > 600.0f) {
      lives--;

      if (lives == 0) {
        gameOver = true;
        ball->velocityX = 0.0f;
        ball->velocityY = 0.0f;
      } else {
        PrepareServe();
      }
    }

    paddle->velocityX = 0.0f;

    if (leftPressed) {
      paddle->velocityX -= 550.0f;
    }

    if (rightPressed) {
      paddle->velocityX += 550.0f;
    }

    if (paddle->minX + paddle->velocityX * dt < 15.0f) {
      paddle->velocityX = (15.0f - paddle->minX) / dt;
    } else if (paddle->maxX + paddle->velocityX * dt > 785.0f) {
      paddle->velocityX = (785.0f - paddle->maxX) / dt;
    }

    if (waitingForServe || gameOver) {
      ball->velocityX = paddle->velocityX;
      ball->velocityY = 0.0f;
      ball->posX = paddle->GetCenterX();
      ball->posY = paddle->minY - ball->radius - 2.0f;
    }
  }

  void OnCollisionStay(Collision &col) override {
    Object *other = nullptr;

    if (col.object1 == ball) {
      other = col.object2;
    } else if (col.object2 == ball) {
      other = col.object1;
    }

    if (other == nullptr) {
      return;
    }

    if (other == paddle) {
      ball->velocityX += paddle->velocityX * 0.2f;

      if (ball->velocityX > 650.0f) {
        ball->velocityX = 650.0f;
      } else if (ball->velocityX < -650.0f) {
        ball->velocityX = -650.0f;
      }

      return;
    }

    for (int i = 0; i < 40; i++) {
      if (other == bricks[i].object && bricks[i].object->active) {
        bricks[i].object->active = false;
        bricksRemaining--;
        score += 100 * level;

        if (bricksRemaining == 0) {
          levelComplete = true;
        }

        return;
      }
    }
  }

  void HandleDraw(sf::RenderWindow &window) override {
    UpdateTitle(window);
    window.clear(sf::Color(14, 18, 30));

    for (int i = 0; i < 40; i++) {
      if (bricks[i].object->active) {
        DrawRect(window, bricks[i].object, bricks[i].color);
      }
    }

    DrawRect(window, paddle, sf::Color(100, 225, 255));

    sf::CircleShape ballShape(ball->radius);
    ballShape.setPosition(ball->posX - ball->radius, ball->posY - ball->radius);
    ballShape.setFillColor(sf::Color::White);
    window.draw(ballShape);

    for (int i = 0; i < lives; i++) {
      sf::CircleShape lifeShape(5.0f);
      lifeShape.setPosition(15.0f + i * 15.0f, 582.0f);
      lifeShape.setFillColor(sf::Color(100, 225, 255));
      window.draw(lifeShape);
    }
  }

private:
  void LaunchBall() {
    waitingForServe = false;
    ball->velocityX = serveDirection * (180.0f + 20.0f * level);
    ball->velocityY = -(400.0f + 30.0f * level);
    serveDirection *= -1.0f;
  }

  void PrepareServe() {
    waitingForServe = true;
    ball->velocityX = 0.0f;
    ball->velocityY = 0.0f;
    ball->posX = paddle->GetCenterX();
    ball->posY = paddle->minY - ball->radius - 2.0f;
  }

  void ResetGame() {
    score = 0;
    lives = 3;
    level = 1;
    serveDirection = 1.0f;
    waitingForServe = false;
    gameOver = false;
    levelComplete = false;
    leftPressed = false;
    rightPressed = false;

    paddle->minX = 325.0f;
    paddle->maxX = 475.0f;
    paddle->minY = 550.0f;
    paddle->maxY = 570.0f;
    paddle->velocityX = 0.0f;
    paddle->velocityY = 0.0f;

    SetupBricks();
    PrepareServe();
  }

  void SetupBricks() {
    bricksRemaining = 40;

    for (int row = 0; row < 5; row++) {
      for (int column = 0; column < 8; column++) {
        int index = row * 8 + column;
        float minX = 48.0f + column * 88.0f;
        float minY = 70.0f + row * 32.0f;

        if (bricks[index].object == nullptr) {
          bricks[index].object = new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, minX, minY, minX + 78.0f, minY + 22.0f);
          objects.addValue(bricks[index].object);
        } else {
          bricks[index].object->minX = minX;
          bricks[index].object->minY = minY;
          bricks[index].object->maxX = minX + 78.0f;
          bricks[index].object->maxY = minY + 22.0f;
        }

        bricks[index].object->active = true;

        if (row == 0) {
          bricks[index].color = sf::Color(255, 90, 90);
        } else if (row == 1) {
          bricks[index].color = sf::Color(255, 170, 70);
        } else if (row == 2) {
          bricks[index].color = sf::Color(255, 225, 90);
        } else if (row == 3) {
          bricks[index].color = sf::Color(90, 210, 150);
        } else {
          bricks[index].color = sf::Color(90, 155, 255);
        }
      }
    }
  }

  sf::String ToString(int value) {
    char string[11];
    int size = 0;

    do {
      string[size] = '0' + value % 10;
      value /= 10;
      size++;
    } while (value > 0);

    for (int i = 0; i < size / 2; i++) {
      char temp = string[i];
      string[i] = string[size - i - 1];
      string[size - i - 1] = temp;
    }

    string[size] = '\0';
    return string;
  }

  void UpdateTitle(sf::RenderWindow &window) {
    sf::String title = "Brick Breaker | Score: ";
    title += ToString(score);
    title += " | Lives: ";
    title += ToString(lives);
    title += " | Level: ";
    title += ToString(level);

    if (gameOver) {
      title += " | GAME OVER - R to restart";
    } else if (waitingForServe) {
      title += " | SPACE to launch";
    }

    window.setTitle(title);
  }

  void DrawRect(sf::RenderWindow &window, Rect *rect, sf::Color color) {
    sf::RectangleShape shape;
    shape.setPosition(rect->minX, rect->minY);
    shape.setSize(sf::Vector2f(rect->maxX - rect->minX, rect->maxY - rect->minY));
    shape.setFillColor(color);
    window.draw(shape);
  }
};

} // namespace

int main() {
  Game game("Ochengine: Brick Breaker", 800, 600, 120);
  game.scenes.addValue(new BrickBreakerScene());

  game.ToggleWindow(true);
  game.window.setFramerateLimit(60);
  game.window.setKeyRepeatEnabled(false);
  game.PlayScene(0, 0.0f, IntegrationType::Euler);

  return 0;
}
