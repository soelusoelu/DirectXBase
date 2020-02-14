#include "SceneBase.h"
#include "../Camera/Camera.h"
#include "../Device/Renderer.h"
#include "../System/Game.h"

SceneBase::SceneBase() :
    mRenderer(nullptr),
    mCamera(nullptr),
    mNext(nullptr) {
}

SceneBase::~SceneBase() = default;

void SceneBase::nextScene(std::shared_ptr<SceneBase> next) {
    mNext = next;
}

void SceneBase::set(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) {
    mRenderer = renderer;
    mCamera = camera;
}

std::shared_ptr<SceneBase> SceneBase::getNextScene() const {
    return mNext;
}
