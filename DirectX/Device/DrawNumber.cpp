#include "DrawNumber.h"
#include "../Actor/Transform2D.h"
#include "../Device/Renderer.h"
#include "../Sprite/Sprite.h"
#include "../System/Game.h"

void DrawNumber::initialize(std::shared_ptr<Renderer> renderer) {
    mSprite = new Sprite(renderer, "number.png");
    mSprite->transform()->setPivot(Pivot::LEFT_TOP);
}

void DrawNumber::end() {
    SAFE_DELETE(mSprite);
}

void DrawNumber::drawNumber(std::shared_ptr<Renderer> renderer, int number, const Vector2& position) {
    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    static constexpr int width = 32; //画像1文字の横幅
    static constexpr int spriteWidth = 512; //画像横幅
    static constexpr float widthRate = static_cast<float>(width) / static_cast<float>(spriteWidth); //画像横幅に対する1文字の比率

    //桁数計算(本当は=1)
    //int digit = 0;
    //for (int i = number; i >= 10; i /= 10) {
    //    digit++;
    //}

    mSprite->transform()->setPosition(position);

    //数字を文字列化し、1文字ずつ取り出す
    for (auto n : std::to_string(number)) {
        auto copy = new Sprite(renderer, "number.png");
        copy->setOnceDraw();
        copy->transform()->setPosition(mSprite->transform()->getPosition());
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        float num = (n - '0') * width;
        num /= spriteWidth;
        copy->setUV(num, 0, num + widthRate, 1);
        copy->transform()->setPivot(Pivot::LEFT_TOP);

        //1文字描画したら1桁分右にずらす
        mSprite->transform()->translate(Vector2(width, 0.f));
    }
}

Sprite* DrawNumber::mSprite = nullptr;
