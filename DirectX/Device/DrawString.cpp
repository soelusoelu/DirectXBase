#include "DrawString.h"
#include "../Device/Renderer.h"
#include "../Sprite/Sprite.h"
#include "../System/Game.h"
#include <iomanip>
#include <sstream>

void DrawString::initialize(std::shared_ptr<Renderer> renderer) {
    mSprite = new Sprite(renderer, "number.png");
    renderer->createTexture("font.png");
}

void DrawString::end() {
    SAFE_DELETE(mSprite);
}

void DrawString::drawNumber(int number, const Vector2& position, Pivot pivot) {
    //�}�C�i�X�͈���Ȃ�
    if (number < 0) {
        number = 0;
    }

    mSprite->transform()->setPosition(position);

    //�����𕶎��񉻂��A1���������o��
    for (auto n : std::to_string(number)) {
        auto copy = new Sprite(mSprite->renderer(), "number.png");
        copy->setOnceDraw();
        copy->transform()->setPosition(mSprite->transform()->getPosition());
        //�����̃e�N�X�`��������1�ɂ���32����64
        //�����ƕ����������Z���A�����l���擾���Ă���
        float num = (n - '0') * WIDTH;
        num /= SPRITE_WIDTH;
        copy->setUV(num, 0.f, num + WIDTH_RATE, 1.f);
        copy->transform()->setPivot(pivot);

        //1�����`�悵����1�����E�ɂ��炷
        mSprite->transform()->translate(Vector2(WIDTH, 0.f));
    }
}

void DrawString::drawNumberRightJustified(int number, const Vector2& position, Pivot pivot) {
    //�����v�Z(�{����=1)
    int digit = 0;
    for (int i = number; i >= 10; i /= 10) {
        digit++;
    }

    auto pos = position;
    pos.x -= WIDTH * digit;
    drawNumber(number, pos, pivot);
}

void DrawString::drawNumber(float number, const Vector2& position, int decimalDigits, Pivot pivot) {
    //�}�C�i�X�͈���Ȃ�
    if (number < 0) {
        number = 0;
    }

    mSprite->transform()->setPosition(position);

    //���������̌����w��
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalDigits) << number;
    std::string num = oss.str();

    //�����𕶎��񉻂��A1���������o��
    for (auto n : num) {
        auto copy = new Sprite(mSprite->renderer(), "number.png");
        copy->setOnceDraw();
        copy->transform()->setPosition(mSprite->transform()->getPosition());
        //�����̃e�N�X�`��������1�ɂ���32����64
        //�����ƕ����������Z���A�����l���擾���Ă���
        if (n == '.') {
            constexpr float num = 10 * WIDTH_RATE; //�s���I�h�͉摜��10�Ԗ�
            copy->setUV(num, 0.f, num + PERIOD_RATE, 1.f);

            //�u.�v�̂Ƃ���1�����̔������炷
            mSprite->transform()->translate(Vector2(PERIOD_WIDTH, 0.f));
        } else {
            float num = (n - '0') * WIDTH;
            num /= SPRITE_WIDTH;
            copy->setUV(num, 0.f, num + WIDTH_RATE, 1.f);

            //1�����`�悵����1�����E�ɂ��炷
            mSprite->transform()->translate(Vector2(WIDTH, 0.f));
        }
        copy->transform()->setPivot(pivot);
    }
}

void DrawString::drawNumberRightJustified(float number, const Vector2& position, int decimalDigits) {
    int digit = 0;
    for (int i = number; i >= 10; i /= 10) {
        digit++;
    }

    auto pos = position;
    pos.x -= WIDTH * digit + WIDTH * decimalDigits;
    pos.x -= PERIOD_WIDTH; //�s���I�h�̕�
    drawNumber(number, pos, decimalDigits, Pivot::LEFT_TOP);
}

Sprite* DrawString::mSprite = nullptr;
