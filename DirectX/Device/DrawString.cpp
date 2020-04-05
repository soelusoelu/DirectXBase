#include "DrawString.h"
#include "../Sprite/Sprite.h"
#include "../System/Game.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

DrawString::DrawString() :
    mNumberSprite(nullptr),
    mFontSprite(nullptr) {
}

DrawString::~DrawString() = default;

void DrawString::initialize() {
    mNumberSprite = std::make_unique<Sprite>(mNumberFileName);
    mFontSprite = std::make_unique<Sprite>(mFontFileName);
}

void DrawString::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["drawString"];
    if (obj.IsObject()) {
        JsonHelper::getString(obj, "number", &mNumberFileName);
        JsonHelper::getString(obj, "font", &mFontFileName);
    }
}

void DrawString::drawAll(const Matrix4 & proj) const {
    for (const auto& param : mParamsInt) {
        drawInt(param, proj);
    }
    for (const auto& param : mParamsFloat) {
        drawFloat(param, proj);
    }
    for (const auto& param : mParamsString) {
        drawString(param, proj);
    }
}

void DrawString::clear() {
    mParamsInt.clear();
    mParamsFloat.clear();
    mParamsString.clear();
}

void DrawString::drawNumber(int number, const Vector2 & position, const Vector2 & scale, Pivot pivot) {
    ParamInt param;
    param.number = number;
    param.position = position;
    param.scale = scale;
    param.pivot = pivot;

    mParamsInt.emplace_back(param);
}

void DrawString::drawNumberRightJustified(int number, const Vector2 & position, const Vector2 & scale, Pivot pivot) {
    //桁数計算(本当は=1)
    int digit = 0;
    for (int i = number; i >= 10; i /= 10) {
        digit++;
    }

    auto pos = position;
    pos.x -= WIDTH * digit * scale.x;
    drawNumber(number, pos, scale, pivot);
}

void DrawString::drawNumber(float number, const Vector2 & position, const Vector2 & scale, int decimalDigits, Pivot pivot) {
    ParamFloat param;
    param.number = number;
    param.position = position;
    param.scale = scale;
    param.decimalDigits = decimalDigits;
    param.pivot = pivot;

    mParamsFloat.emplace_back(param);
}

void DrawString::drawNumberRightJustified(float number, const Vector2 & position, const Vector2 & scale, int decimalDigits) {
    int digit = 0;
    for (int i = number; i >= 10; i /= 10) {
        digit++;
    }

    auto pos = position;
    pos.x -= (WIDTH * digit + WIDTH * decimalDigits) * scale.x;
    pos.x -= PERIOD_WIDTH * scale.x; //ピリオドの分
    drawNumber(number, pos, scale, decimalDigits, Pivot::LEFT_TOP);
}

void DrawString::drawString(const std::string & alphabet, const Vector2 & position, const Vector2 & scale, const Vector3 & color, float alpha, Pivot pivot) {
    ParamString param;
    param.alphabet = alphabet;
    param.position = position;
    param.scale = scale;
    param.color = color;
    param.alpha = alpha;
    param.pivot = pivot;

    mParamsString.emplace_back(param);
}

void DrawString::drawStringRightJustified(const std::string & alphabet, const Vector2 & position, const Vector2 & scale, const Vector3 & color, float alpha, Pivot pivot) {
    auto pos = position;
    pos.x -= alphabet.length() * WIDTH * scale.x;
    drawString(alphabet, pos, scale, color, alpha, Pivot::LEFT_TOP);
}

void DrawString::drawInt(const ParamInt & param, const Matrix4 & proj) const {
    //要素取り出し
    auto number = param.number;
    auto pos = param.position;
    auto scale = param.scale;
    auto pivot = param.pivot;

    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    const float width = WIDTH * scale.x;

    //数字を文字列化し、1文字ずつ取り出す
    for (auto n : std::to_string(number)) {
        mNumberSprite->transform()->setPosition(pos);
        mNumberSprite->transform()->setScale(scale);
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        float num = (n - '0') * WIDTH;
        num /= SPRITE_WIDTH;
        mNumberSprite->setUV(num, 0.f, num + WIDTH_RATE, 1.f);
        mNumberSprite->transform()->setPivot(pivot);

        //ワールド座標を更新するため
        mNumberSprite->update();
        mNumberSprite->draw(proj);

        //1文字描画したら1桁分右にずらす
        pos.x += width;
    }
}

void DrawString::drawFloat(const ParamFloat & param, const Matrix4 & proj) const {
    //要素取り出し
    auto number = param.number;
    auto pos = param.position;
    auto scale = param.scale;
    auto decimalDigits = param.decimalDigits;
    auto pivot = param.pivot;

    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    const float width = WIDTH * scale.x;
    const float periodWidth = PERIOD_WIDTH * scale.x;

    //小数部分の桁数指定
    auto num = StringUtil::floatToString(number, decimalDigits);

    //数字を文字列化し、1文字ずつ取り出す
    for (auto n : num) {
        mNumberSprite->transform()->setPosition(pos);
        mNumberSprite->transform()->setScale(scale);
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        float widthAmount = 0.f;
        if (n == '.') {
            constexpr float num = 10 * WIDTH_RATE; //ピリオドは画像の10番目
            mNumberSprite->setUV(num, 0.f, num + PERIOD_RATE, 1.f);

            //「.」のときは1文字の半分ずらす
            widthAmount = periodWidth;
        } else {
            float num = (n - '0') * WIDTH;
            num /= SPRITE_WIDTH;
            mNumberSprite->setUV(num, 0.f, num + WIDTH_RATE, 1.f);

            //1文字描画したら1桁分右にずらす
            widthAmount = width;
        }
        mNumberSprite->transform()->setPivot(pivot);

        //ワールド座標を更新するため
        mNumberSprite->update();
        mNumberSprite->draw(proj);

        pos.x += widthAmount;
    }
}

void DrawString::drawString(const ParamString & param, const Matrix4 & proj) const {
    //要素取り出し
    auto alphabet = param.alphabet;
    auto pos = param.position;
    auto scale = param.scale;
    auto color = param.color;
    auto alpha = param.alpha;
    auto pivot = param.pivot;

    const float width = WIDTH * scale.x;

    for (const auto& c : alphabet) {
        mFontSprite->transform()->setPosition(pos);
        mFontSprite->transform()->setScale(scale);
        mFontSprite->setColor(color);
        mFontSprite->setAlpha(alpha);

        int t = c;
        t = Math::clamp<int>(t, 32, 127);
        t -= 32;

        float left = t % WIDTH_CHAR_COUNT;
        left /= WIDTH_CHAR_COUNT;
        float top = t / WIDTH_CHAR_COUNT;
        top /= HEIGHT_CHAR_COUNT;
        mFontSprite->setUV(left, top, left + WIDTH_RATE, top + FONT_HEIGHT_RATE);
        mFontSprite->transform()->setPivot(pivot);

        //ワールド座標を更新するため
        mFontSprite->update();
        mFontSprite->draw(proj);

        pos.x += width;
    }
}
