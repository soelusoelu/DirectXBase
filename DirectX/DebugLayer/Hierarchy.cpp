#include "Hierarchy.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/DrawString.h"
#include "../Input/Input.h"
#include "../Input/Mouse.h"
#include "../System/Window.h"
#include "../UI/UIButton.h"
#include "../Utility/LevelLoader.h"

Hierarchy::Hierarchy(DrawString* drawString) :
    mDrawString(drawString),
    mNumRowsToDisplay(0),
    mInspectorPositionX(0.f),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mOffsetCharCountX(0),
    mOffsetCharCountY(0),
    mCharWidth(0.f),
    mCharHeight(0.f) {
}

Hierarchy::~Hierarchy() = default;

void Hierarchy::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["hierarchy"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
        JsonHelper::getInt(obj, "offsetCharCountY", &mOffsetCharCountY);
    }
    const auto& inspector = inObj["inspector"];
    if (inspector.IsObject()) {
        JsonHelper::getFloat(inspector, "inspectorPositionX", &mInspectorPositionX);
    }
}

void Hierarchy::initialize() {
    mCharWidth = DrawString::WIDTH * mScale.x;
    mCharHeight = DrawString::HEIGHT * mScale.y;

    mPosition = Vector2(Window::width(), 0.f);
    mPosition += Vector2(mOffsetCharCountX * mCharWidth, mOffsetCharCountY * mCharHeight);

    //画面内に収まる行数
    mNumRowsToDisplay = (Window::debugHeight() - mPosition.y) / mCharHeight;

    mButtons.resize(mNumRowsToDisplay);
    auto pos = mPosition;
    for (auto&& b : mButtons) {
        //全ボタンに当たり判定をつける
        b.first = std::make_unique<UIButton>(nullptr, pos, Vector2(mInspectorPositionX - pos.x, mCharHeight));
        pos.y += mCharHeight;
    }
}

void Hierarchy::update(const std::list<std::shared_ptr<Actor>> actors) {
    auto itr = mButtons.begin();
    for (const auto& actor : actors) {
        //アクターの数がボタンの数より多いときは無視
        if (itr == mButtons.end()) {
            break;
        }
        itr->second = actor;
        ++itr;
    }

    const auto& mousePos = Input::mouse()->getMousePosition();
    if (Input::mouse()->getMouseDown(MouseCode::LeftButton)) {
        for (const auto& b : mButtons) {
            if (!b.first->containsPoint(mousePos)) {
                continue;
            }
            auto actor = b.second.lock();
            if (actor) {
                Debug::inspector()->setTarget(actor);
                break;
            }
        }
    }
}

void Hierarchy::drawActors() {
    auto drawPos = mPosition;
    for (const auto& b : mButtons) {
        auto actor = b.second.lock();
        //アクターが登録されてなかったら終了
        if (!actor) {
            break;
        }

        mDrawString->drawString(actor->tag(), drawPos, mScale);
        drawPos.y += mCharHeight;
    }
}
