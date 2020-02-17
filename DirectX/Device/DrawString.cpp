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
    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    mSprite->transform()->setPosition(position);

    //数字を文字列化し、1文字ずつ取り出す
    for (auto n : std::to_string(number)) {
        auto copy = new Sprite(mSprite->renderer(), "number.png");
        copy->setOnceDraw();
        copy->transform()->setPosition(mSprite->transform()->getPosition());
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        float num = (n - '0') * WIDTH;
        num /= SPRITE_WIDTH;
        copy->setUV(num, 0.f, num + WIDTH_RATE, 1.f);
        copy->transform()->setPivot(pivot);

        //1文字描画したら1桁分右にずらす
        mSprite->transform()->translate(Vector2(WIDTH, 0.f));
    }
}

void DrawString::drawNumberRightJustified(int number, const Vector2& position, Pivot pivot) {
    //桁数計算(本当は=1)
    int digit = 0;
    for (int i = number; i >= 10; i /= 10) {
        digit++;
    }

    auto pos = position;
    pos.x -= WIDTH * digit;
    drawNumber(number, pos, pivot);
}

void DrawString::drawNumber(float number, const Vector2& position, int decimalDigits, Pivot pivot) {
    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    mSprite->transform()->setPosition(position);

    //小数部分の桁数指定
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalDigits) << number;
    std::string num = oss.str();

    //数字を文字列化し、1文字ずつ取り出す
    for (auto n : num) {
        auto copy = new Sprite(mSprite->renderer(), "number.png");
        copy->setOnceDraw();
        copy->transform()->setPosition(mSprite->transform()->getPosition());
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        if (n == '.') {
            constexpr float num = 10 * WIDTH_RATE; //ピリオドは画像の10番目
            copy->setUV(num, 0.f, num + PERIOD_RATE, 1.f);

            //「.」のときは1文字の半分ずらす
            mSprite->transform()->translate(Vector2(PERIOD_WIDTH, 0.f));
        } else {
            float num = (n - '0') * WIDTH;
            num /= SPRITE_WIDTH;
            copy->setUV(num, 0.f, num + WIDTH_RATE, 1.f);

            //1文字描画したら1桁分右にずらす
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
    pos.x -= PERIOD_WIDTH; //ピリオドの分
    drawNumber(number, pos, decimalDigits, Pivot::LEFT_TOP);
}

Sprite* DrawString::mSprite = nullptr;
