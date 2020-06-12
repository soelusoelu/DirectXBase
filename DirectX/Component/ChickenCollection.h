#pragma once

#include "Component.h"
#include "../Input/Input.h"
#include <memory>

class FriedChickenComponent;
class SoundComponent;
class Time;

class ChickenCollection : public Component {
    using ChickenPtr = std::shared_ptr<FriedChickenComponent>;

public:
    ChickenCollection();
    ~ChickenCollection();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    //回収を試みる
    bool tryCollection(const ChickenPtr& target);

private:
    //回収キーを押したか
    bool pressedCollectionKey() const;
    //回収条件を満たしたか
    bool successCollection(const FriedChickenComponent& target) const;
    //サウンド再生準備を進める
    void setUpSound();
    //プレイヤーの足元の唐揚げを回収する
    void collection(const ChickenPtr& target);

private:
    std::shared_ptr<SoundComponent> mSound;
    std::unique_ptr<Time> mReplaySoundTimer;
    KeyCode mCollectionKey;
    JoyCode mCollectionPad;
};
