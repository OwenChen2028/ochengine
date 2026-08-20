#include "ochengine/game.hpp"
#include <SFML/Graphics.hpp>

namespace {

struct Brick {
  Rect *object;
  sf::Color color;
};

struct ScreenPoint {
  float x;
  float y;
  float depth;
};

struct DrawItem {
  Object *object;
  sf::Color color;
  float height;
};

struct BrickBreaker3DScene : Scene {
  Rect *paddle;
  Circle *ball;
  Brick bricks[40];

  bool leftPressed;
  bool rightPressed;
  bool waitingForServe;
  bool gameOver;
  bool levelComplete;
  bool paddleColliding;
  bool paddleCollidingLastUpdate;

  int score;
  int lives;
  int level;
  int bricksRemaining;

  float serveDirection;

  float cameraY;
  float cameraZ;
  float cameraForwardY;
  float cameraForwardZ;
  float cameraUpY;
  float cameraUpZ;

  BrickBreaker3DScene() {
    paddle = nullptr;
    ball = nullptr;

    for (int i = 0; i < 40; i++) {
      bricks[i].object = nullptr;
    }

    cameraY = 1050.0f;
    cameraZ = 950.0f;

    float cameraLength = FindSqrt(FindSquare(cameraY) + FindSquare(cameraZ));
    cameraForwardY = -cameraY / cameraLength;
    cameraForwardZ = -cameraZ / cameraLength;
    cameraUpY = cameraForwardZ;
    cameraUpZ = -cameraForwardY;

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
    paddleCollidingLastUpdate = paddleColliding;
    paddleColliding = false;

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
      if (!paddleCollidingLastUpdate && !paddleColliding) {
        ball->velocityX += paddle->velocityX * 0.2f;

        if (ball->velocityX > 650.0f) {
          ball->velocityX = 650.0f;
        } else if (ball->velocityX < -650.0f) {
          ball->velocityX = -650.0f;
        }
      }

      paddleColliding = true;
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
    window.clear(sf::Color(7, 10, 22));

    DrawBoard(window);
    DrawBox(window, static_cast<Rect *>(objects.getValue(2)), 46.0f, sf::Color(52, 70, 108));
    DrawBallShadow(window);

    DrawItem drawItems[42];
    int drawItemCount = 0;

    for (int i = 0; i < 40; i++) {
      if (bricks[i].object->active) {
        drawItems[drawItemCount].object = bricks[i].object;
        drawItems[drawItemCount].color = bricks[i].color;
        drawItems[drawItemCount].height = 34.0f;
        drawItemCount++;
      }
    }

    drawItems[drawItemCount].object = paddle;
    drawItems[drawItemCount].color = sf::Color(75, 210, 245);
    drawItems[drawItemCount].height = 30.0f;
    drawItemCount++;

    drawItems[drawItemCount].object = ball;
    drawItems[drawItemCount].color = sf::Color::White;
    drawItems[drawItemCount].height = 0.0f;
    drawItemCount++;

    SortDrawItems(drawItems, drawItemCount);

    for (int i = 0; i < drawItemCount; i++) {
      if (drawItems[i].object == ball) {
        DrawBall(window);
      } else {
        DrawBox(window, static_cast<Rect *>(drawItems[i].object), drawItems[i].height, drawItems[i].color);
      }
    }

    DrawBox(window, static_cast<Rect *>(objects.getValue(3)), 46.0f, sf::Color(52, 70, 108));
    DrawBox(window, static_cast<Rect *>(objects.getValue(4)), 46.0f, sf::Color(52, 70, 108));
    DrawForeground(window);
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
    paddleColliding = false;
    paddleCollidingLastUpdate = false;
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
          bricks[index].color = sf::Color(255, 82, 105);
        } else if (row == 1) {
          bricks[index].color = sf::Color(255, 143, 72);
        } else if (row == 2) {
          bricks[index].color = sf::Color(255, 215, 74);
        } else if (row == 3) {
          bricks[index].color = sf::Color(82, 218, 160);
        } else {
          bricks[index].color = sf::Color(78, 142, 255);
        }
      }
    }
  }

  ScreenPoint Project(float x, float y, float z) {
    float relativeX = x - 400.0f;
    float relativeY = y - 300.0f - cameraY;
    float relativeZ = z - cameraZ;

    ScreenPoint point;
    point.depth = relativeY * cameraForwardY + relativeZ * cameraForwardZ;

    float cameraX = relativeX;
    float cameraUp = relativeY * cameraUpY + relativeZ * cameraUpZ;
    float focalLength = 1050.0f;

    point.x = 550.0f + focalLength * cameraX / point.depth;
    point.y = 335.0f - focalLength * cameraUp / point.depth;

    return point;
  }

  sf::Color Shade(sf::Color color, int numerator, int denominator) { return sf::Color(color.r * numerator / denominator, color.g * numerator / denominator, color.b * numerator / denominator); }

  void DrawQuad(sf::RenderWindow &window, ScreenPoint point1, ScreenPoint point2, ScreenPoint point3, ScreenPoint point4, sf::Color color) {
    sf::ConvexShape shape;
    shape.setPointCount(4);
    shape.setPoint(0, sf::Vector2f(point1.x, point1.y));
    shape.setPoint(1, sf::Vector2f(point2.x, point2.y));
    shape.setPoint(2, sf::Vector2f(point3.x, point3.y));
    shape.setPoint(3, sf::Vector2f(point4.x, point4.y));
    shape.setFillColor(color);
    window.draw(shape);
  }

  void DrawLine(sf::RenderWindow &window, float x1, float y1, float x2, float y2, sf::Color color) {
    ScreenPoint point1 = Project(x1, y1, 1.0f);
    ScreenPoint point2 = Project(x2, y2, 1.0f);

    sf::Vertex line[2];
    line[0] = sf::Vertex(sf::Vector2f(point1.x, point1.y), color);
    line[1] = sf::Vertex(sf::Vector2f(point2.x, point2.y), color);
    window.draw(line, 2, sf::Lines);
  }

  void DrawBoard(sf::RenderWindow &window) {
    DrawQuad(window, Project(-45.0f, -35.0f, -12.0f), Project(845.0f, -35.0f, -12.0f), Project(845.0f, 635.0f, -12.0f), Project(-45.0f, 635.0f, -12.0f), sf::Color(20, 28, 48));
    DrawQuad(window, Project(0.0f, 0.0f, 0.0f), Project(800.0f, 0.0f, 0.0f), Project(800.0f, 615.0f, 0.0f), Project(0.0f, 615.0f, 0.0f), sf::Color(20, 47, 74));

    for (int x = 100; x < 800; x += 100) {
      DrawLine(window, x, 20.0f, x, 615.0f, sf::Color(27, 61, 91));
    }

    for (int y = 100; y < 600; y += 100) {
      DrawLine(window, 0.0f, y, 800.0f, y, sf::Color(27, 61, 91));
    }
  }

  void DrawBox(sf::RenderWindow &window, Rect *rect, float height, sf::Color color) {
    ScreenPoint bottomNearLeft = Project(rect->minX, rect->maxY, 0.0f);
    ScreenPoint bottomNearRight = Project(rect->maxX, rect->maxY, 0.0f);
    ScreenPoint topNearLeft = Project(rect->minX, rect->maxY, height);
    ScreenPoint topNearRight = Project(rect->maxX, rect->maxY, height);

    ScreenPoint bottomFarLeft = Project(rect->minX, rect->minY, 0.0f);
    ScreenPoint bottomFarRight = Project(rect->maxX, rect->minY, 0.0f);
    ScreenPoint topFarLeft = Project(rect->minX, rect->minY, height);
    ScreenPoint topFarRight = Project(rect->maxX, rect->minY, height);

    DrawQuad(window, bottomNearLeft, bottomNearRight, topNearRight, topNearLeft, Shade(color, 2, 5));

    if (rect->GetCenterX() < 400.0f) {
      DrawQuad(window, bottomNearRight, bottomFarRight, topFarRight, topNearRight, Shade(color, 3, 5));
    } else {
      DrawQuad(window, bottomFarLeft, bottomNearLeft, topNearLeft, topFarLeft, Shade(color, 3, 5));
    }

    DrawQuad(window, topFarLeft, topFarRight, topNearRight, topNearLeft, color);
  }

  void DrawBallShadow(sf::RenderWindow &window) {
    ScreenPoint point = Project(ball->posX + 5.0f, ball->posY + 7.0f, 1.0f);
    float radius = 1050.0f * 13.0f / point.depth;

    sf::CircleShape shadow(radius, 32);
    shadow.setOrigin(radius, radius);
    shadow.setPosition(point.x, point.y);
    shadow.setScale(1.0f, 0.42f);
    shadow.setFillColor(sf::Color(2, 5, 12, 145));
    window.draw(shadow);
  }

  void DrawBall(sf::RenderWindow &window) {
    ScreenPoint point = Project(ball->posX, ball->posY, 13.0f);
    float radius = 1050.0f * 13.0f / point.depth;

    sf::CircleShape ballShape(radius, 40);
    ballShape.setOrigin(radius, radius);
    ballShape.setPosition(point.x, point.y);
    ballShape.setFillColor(sf::Color(235, 245, 255));
    ballShape.setOutlineThickness(1.5f);
    ballShape.setOutlineColor(sf::Color(105, 155, 205));
    window.draw(ballShape);

    sf::CircleShape highlight(radius * 0.24f, 24);
    highlight.setOrigin(radius * 0.24f, radius * 0.24f);
    highlight.setPosition(point.x - radius * 0.31f, point.y - radius * 0.34f);
    highlight.setFillColor(sf::Color(255, 255, 255, 210));
    window.draw(highlight);
  }

  void SortDrawItems(DrawItem *drawItems, int drawItemCount) {
    for (int i = 1; i < drawItemCount; i++) {
      DrawItem current = drawItems[i];
      int j = i - 1;

      while (j >= 0 && drawItems[j].object->GetCenterY() > current.object->GetCenterY()) {
        drawItems[j + 1] = drawItems[j];
        j--;
      }

      drawItems[j + 1] = current;
    }
  }

  void DrawForeground(sf::RenderWindow &window) {
    ScreenPoint left = Project(0.0f, 615.0f, 0.0f);
    ScreenPoint right = Project(800.0f, 615.0f, 0.0f);

    sf::Vertex line[2];
    line[0] = sf::Vertex(sf::Vector2f(left.x, left.y), sf::Color(75, 105, 145));
    line[1] = sf::Vertex(sf::Vector2f(right.x, right.y), sf::Color(75, 105, 145));
    window.draw(line, 2, sf::Lines);
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
    sf::String title = "Brick Breaker 3D | Score: ";
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
};

} // namespace

int main() {
  Game game("Ochengine: Brick Breaker 3D", 1100, 720, 120);
  game.scenes.addValue(new BrickBreaker3DScene());

  game.ToggleWindow(true);
  game.window.setFramerateLimit(60);
  game.window.setKeyRepeatEnabled(false);
  game.PlayScene(0, 0.0f, IntegrationType::Euler);

  return 0;
}
