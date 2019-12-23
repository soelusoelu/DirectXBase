#pragma once

#include <memory>
#include <list>

class Renderer;
class Sprite;

class SpriteManager {
public:
    SpriteManager();
    ~SpriteManager();
    void update();
    void draw(std::shared_ptr<Renderer> renderer);
    void add(Sprite* add);
    void add(std::shared_ptr<Sprite> add);
    void clear();

private:
    void remove();
    void sortByZ(); //未実装

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;

private:
    std::list<std::shared_ptr<Sprite>> mSprites;
};

