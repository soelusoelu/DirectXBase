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
    FixedDebugInformation(DrawString* drawString);
    ~FixedDebugInformation();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    //フレームレート表示
    void drawFPS(float fps) const;

private:
    DrawString* mDrawString;
    //文字のスケール
    Vector2 mScale;
    //画面に表示するログの行数
    int mNumRowsToDisplay;
    //FPSを表示する位置 固定
    Vector2 mFPSPos;
};
