#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class GameObject;
class PlayerComponent;
class FriedChickenComponent;
class SoundComponent;
enum class KeyCode;
enum class JoyCode;

//プレイヤーと唐揚げの情報をやり取りするクラス
class PlayerChickenConnection : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ChickenPtr = std::shared_ptr<FriedChickenComponent>;

public:
    PlayerChickenConnection();
    ~PlayerChickenConnection();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setPlayer(const GameObject& player);
    void setChicken(const ChickenPtr& chicken);
    const ChickenPtr& getChicken() const;
    //ジャンプターゲットが有効か
    bool isJumpTarget() const;
    //ジャンプターゲットのトップポジション
    Vector3 getJumpTargetTopPos() const;
    void setPlayerJumpTarget(const ChickenPtr& chicken);

private:
    void setPlayerPosOnTheChicken(const FriedChickenComponent& chicken);
    void setChickenPosUnderThePlayer();
    void trackingJumpTarget();
    void rollChicken();
    void setPlayerPosOnTheJumpTarget();
    void collection();
    void tooBurntUnderThePlayer();

private:
    std::shared_ptr<PlayerComponent> mPlayer;
    //プレイヤーの足元の唐揚げ
    ChickenPtr mChicken;
    //プレイヤーのジャンプターゲット
    ChickenPtr mJumpTarget;

    //唐揚げのメッシュ半径
    float mChickenRadius;

    std::shared_ptr<SoundComponent> mSound;

    KeyCode mCollectionKey;
    JoyCode mCollectionPad;
    bool mIsJumpRoll;
};
