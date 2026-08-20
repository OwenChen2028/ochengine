#include "ochengine/game.hpp"

namespace {

struct SimulationScene : Scene {
  SimulationScene() { Reset(); }

  void ProcessEvent(sf::Event event) override {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
      Reset();
    }
  }

private:
  void Reset() {
    for (int i = 0; i < objects.getSize(); i++) {
      delete objects.getValue(i);
    }
    objects = Container<Object *>();

    objects.addValue(new Circle(2.0f, 1.0f, 100.0f, 0.0f, 500.0f, 25.0f, 100.0f, 100.0f));
    objects.addValue(new Circle(2.0f, 1.0f, -100.0f, 0.0f, 500.0f, 25.0f, 100.0f, 100.0f));

    objects.addValue(new Rect(2.5f, 1.0f, -100.0f, 0.0f, 500.0f, 250.0f, 250.0f, 350.0f, 300.0f));
    objects.addValue(new Rect(2.5f, 1.0f, 100.0f, 0.0f, 500.0f, 250.0f, 250.0f, 350.0f, 300.0f));

    objects.addValue(new Rect(5.0f, 1.0f, -100.0f, 0.0f, 500.0f, 350.0f, 350.0f, 450.0f, 450.0f));
    objects.addValue(new Circle(5.0f, 1.0f, 100.0f, 0.0f, 500.0f, 50.0f, 400.0f, 400.0f));

    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 800.0f,
                              10.0f));                                                      // top boundary
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 590.0f, 800.0f, 600.0f)); // bottom boundary
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f,
                              600.0f));                                                     // left boundary
    objects.addValue(new Rect(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 790.0f, 0.0f, 800.0f, 600.0f)); // right boundary
  }
};

} // namespace

int main() {
  Game game("Simulation Demo", 800, 600);
  game.scenes.addValue(new SimulationScene());

  game.ToggleWindow(true);
  game.PlayScene(0);

  return 0;
}
