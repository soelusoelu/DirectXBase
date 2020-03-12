#pragma once

#include "../Utility/Math.h"
#include <list>
#include <memory>
#include <string>

class Actor;
class DrawString;

class FixedDebugInformation {
public:
    FixedDebugInformation(std::shared_ptr<DrawString> drawString);
    ~FixedDebugInformation();
    //フレームレート表示
    void drawFPS(float fps) const;
    //マネージャーに登録されてる全アクターを表示
    void drawActors(std::list<std::shared_ptr<Actor>> actors);

private:
    std::shared_ptr<DrawString> mDrawString;
    static const Vector2 SCALE;
    static const Vector2 FPS_POS;
    static const Vector2 ACTORS_POS;
    static const int NUM_ROWS_TO_DISPLAY;
};
