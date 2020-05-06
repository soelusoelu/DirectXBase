#pragma once

#include "Component.h"
#include <string>

class SoundComponent : public Component {
public:
    SoundComponent(std::shared_ptr<GameObject> owner);
    ~SoundComponent();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    void play();
    void playOneShot(const std::string& fileName, float volumeScale = 1.f);

private:
    std::string mFileName;
    float mVolume;
};
