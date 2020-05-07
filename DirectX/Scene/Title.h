#pragma once

#include "SceneBase.h"
#include <memory>

class TitleComponent;

class Title : public SceneBase {
public:
    Title();
    ~Title();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<TitleComponent> mTitle;
};
