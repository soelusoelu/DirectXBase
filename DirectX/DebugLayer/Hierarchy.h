#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>

class Actor;
class DrawString;

class Hierarchy {
public:
    Hierarchy(DrawString* drawString);
    ~Hierarchy();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    //マネージャーに登録されてる全アクターを表示
    void drawActors(const std::list<std::shared_ptr<Actor>> actors);

private:
    DrawString* mDrawString;
    //表示する位置
    Vector2 mPosition;
    //文字のスケール
    Vector2 mScale;
    //表示位置をずらす文字数
    int mOffsetCharCountX;
    int mOffsetCharCountY;
    //1文字の大きさ
    float mCharWidth;
    float mCharHeight;
};
