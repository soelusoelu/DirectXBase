#include "Result.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Math/Math.h"

Result::Result(std::shared_ptr<GameObject> owner) :
    Component(owner, "Result"),
    mScore(0) {
}

Result::~Result() = default;

void Result::update() {
    auto ds = owner()->renderer()->getDrawString();
    ds->drawString("Score", Vector2(500.f, 400.f));
    ds->drawNumber(mScore, Vector2(500.f, 500.f));
}

void Result::setScore(int score) {
    mScore = score;
}
