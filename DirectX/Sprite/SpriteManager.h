#pragma once

#include "../Math/Math.h"
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
    void draw(const Matrix4& proj);
    void add(SpritePtr add);
    void clear();

private:
    void remove();

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;

private:
    SpritePtrList mSprites;
};
