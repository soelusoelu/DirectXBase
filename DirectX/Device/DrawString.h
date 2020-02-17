#pragma once

#include "../Actor/Transform2D.h"
#include "../Utility/Math.h"
#include <memory>
#include <string>

class Renderer;
class Sprite;

class DrawString {
public:
    static void initialize(std::shared_ptr<Renderer> renderer);
    static void end();
    static void drawNumber(int number, const Vector2& position, Pivot pivot = Pivot::LEFT_TOP);
    static void drawNumberRightJustified(int number, const Vector2& position, Pivot pivot = Pivot::RIGHT_TOP);
    static void drawNumber(float number, const Vector2& position, int decimalDigits = 1, Pivot pivot = Pivot::LEFT_TOP);
    static void drawNumberRightJustified(float number, const Vector2& position, int decimalDigits = 1);

private:
    static Sprite* mSprite;
    static constexpr int WIDTH = 32; //�摜1�����̉���
    static constexpr int HEIGHT = 64; //�摜1�����̏c��
    static constexpr int SPRITE_WIDTH = 512; //�摜����
    static constexpr float WIDTH_RATE = static_cast<float>(WIDTH) / static_cast<float>(SPRITE_WIDTH); //�摜�����ɑ΂���1�����̔䗦
    static constexpr int PERIOD_WIDTH = WIDTH / 2.f; //�s���I�h�̉���
    static constexpr float PERIOD_RATE = static_cast<float>(PERIOD_WIDTH) / static_cast<float>(SPRITE_WIDTH); //�摜�����ɑ΂���1�����̔䗦
};
