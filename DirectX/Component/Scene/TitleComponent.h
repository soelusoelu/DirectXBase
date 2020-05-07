#pragma once

#include "../Component.h"

enum class KeyCode;
enum class JoyCode;

class TitleComponent : public Component {
public:
    TitleComponent(const std::shared_ptr<GameObject>& owner);
    ~TitleComponent();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    bool isEnd() const;

private:
    KeyCode mEnterKey;
    JoyCode mEnterPad;
    bool mIsEnd;
};
