#pragma once

#include <memory>
#include <list>

class Renderer;
class Sprite;

class SpriteManager {
    using SpritePtr = std::shared_ptr<Sprite>;
    using SpritePtrList = std::list<SpritePtr>;

public:
    SpriteManager();
    ~SpriteManager();
    void update();
    void draw();
    void add(SpritePtr add);
    void clear();

private:
    void remove();

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;

private:
    SpritePtrList mSprites;
};
