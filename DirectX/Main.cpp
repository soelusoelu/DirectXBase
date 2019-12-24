#include "Main.h"
#include "Scene/SceneManager.h"

Main::Main(std::shared_ptr<Renderer> renderer) :
    mScene(std::make_unique<SceneManager>(renderer)) {
}

Main::~Main() = default;

void Main::update() {
    mScene->update();
}

void Main::draw() const {
    mScene->draw();
}
