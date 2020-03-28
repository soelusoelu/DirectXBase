#pragma once

#include "UI.h"
#include "../Math/Math.h"
#include <memory>

class Renderer;

class JumpTarget : public UI {
public:
    JumpTarget(std::shared_ptr<Renderer> renderer);
    ~JumpTarget();
    virtual void start() override;
    virtual void updateUI() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    void setTargetPosition(const Vector2& pos);

private:
    Vector2 mPosition;
    Vector2 mTextureScale;
};
