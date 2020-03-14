#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>

class Actor;
class DrawString;

class FixedDebugInformation {
public:
    FixedDebugInformation(std::shared_ptr<DrawString> drawString);
    ~FixedDebugInformation();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    //フレームレート表示
    void drawFPS(float fps) const;
    //マネージャーに登録されてる全アクターを表示
    void drawActors(std::list<std::shared_ptr<Actor>> actors);

private:
    std::shared_ptr<DrawString> mDrawString;
    //文字のスケール
    Vector2 mScale;
    //"Actors"を表示する位置
    Vector2 mActorsPos;
    //画面に表示するログの行数
    int mNumRowsToDisplay;
    //FPSを表示する位置 固定
    static const Vector2 FPS_POS;
};
