#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <string>

class Renderer;
class Sprite;

class DrawNumber {
public:
    //画像の事前読み込み
    static void initialize(std::shared_ptr<Renderer> renderer);
    static void end();
    static void drawNumber(std::shared_ptr<Renderer> renderer, int number, const Vector2& position);

private:
    static Sprite* mSprite;
};
