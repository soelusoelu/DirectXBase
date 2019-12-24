#pragma once

#include <memory>

class Renderer;
class SceneManager;

class Main {
public:
    Main(std::shared_ptr<Renderer> renderer);
    ~Main();
    void update();
    void draw() const;

private:
    std::unique_ptr<SceneManager> mScene;
};

