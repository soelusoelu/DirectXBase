#pragma once

#include "../Actor/Transform2D.h"
#include "../Utility/Math.h"
#include <list>
#include <memory>
#include <string>

class Renderer;
class Sprite;

class DrawString {
public:
    DrawString();
    ~DrawString();
    void initialize(std::shared_ptr<Renderer> renderer, const std::string& number, const std::string& font);
    void drawAll(const Matrix4& proj);

    void drawNumber(
        int number,
        const Vector2& position,
        const Vector2& scale = Vector2::one,
        Pivot pivot = Pivot::LEFT_TOP
    );
    void drawNumberRightJustified(
        int number,
        const Vector2& position,
        const Vector2& scale = Vector2::one,
        Pivot pivot = Pivot::RIGHT_TOP
    );
    void drawNumber(
        float number,
        const Vector2& position,
        const Vector2& scale = Vector2::one,
        int decimalDigits = 1,
        Pivot pivot = Pivot::LEFT_TOP
    );
    void drawNumberRightJustified(
        float number,
        const Vector2& position,
        const Vector2& scale = Vector2::one,
        int decimalDigits = 1
    );
    void drawString(
        const std::string& alphabet,
        const Vector2& position,
        const Vector2& scale = Vector2::one,
        const Vector3& color = ColorPalette::white,
        Pivot pivot = Pivot::LEFT_TOP
    );
    void drawStringRightJustified(
        const std::string& alphabet,
        const Vector2& position,
        const Vector2& scale = Vector2::one,
        const Vector3& color = ColorPalette::white,
        Pivot pivot = Pivot::RIGHT_TOP
    );

private:
    struct ParamInt {
        int number;
        Vector2 position;
        Vector2 scale;
        Pivot pivot;
    };
    struct ParamFloat {
        float number;
        Vector2 position;
        Vector2 scale;
        int decimalDigits;
        Pivot pivot;
    };
    struct ParamString {
        std::string alphabet;
        Vector2 position;
        Vector2 scale;
        Vector3 color;
        Pivot pivot;
    };

    void drawInt(const ParamInt& param, const Matrix4& proj) const;
    void drawFloat(const ParamFloat& param, const Matrix4& proj) const;
    void drawString(const ParamString& param, const Matrix4& proj) const;

public:
    static constexpr int WIDTH = 32; //画像1文字の横幅
    static constexpr int HEIGHT = 64; //画像1文字の縦幅

private:
    std::unique_ptr<Sprite> mNumberSprite;
    std::unique_ptr<Sprite> mFontSprite;
    std::list<ParamInt> mParamsInt;
    std::list<ParamFloat> mParamsFloat;
    std::list<ParamString> mParamsString;

    static constexpr int SPRITE_WIDTH = 512; //画像横幅
    static constexpr float WIDTH_RATE = static_cast<float>(WIDTH) / static_cast<float>(SPRITE_WIDTH); //画像横幅に対する1文字の比率
    static constexpr int PERIOD_WIDTH = WIDTH / 2.f; //ピリオドの横幅
    static constexpr float PERIOD_RATE = static_cast<float>(PERIOD_WIDTH) / static_cast<float>(SPRITE_WIDTH); //画像横幅に対する1文字の比率

    static constexpr int FONT_HEIGHT = 384; //フォント画像の縦幅
    static constexpr int WIDTH_CHAR_COUNT = SPRITE_WIDTH / WIDTH; //フォント画像の画像横の文字数
    static constexpr int HEIGHT_CHAR_COUNT = FONT_HEIGHT / HEIGHT;
    static constexpr float FONT_HEIGHT_RATE = static_cast<float>(HEIGHT) / static_cast<float>(FONT_HEIGHT);
};
