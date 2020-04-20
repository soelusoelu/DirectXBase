#pragma once

class GameObject;
class PlayerComponent;
class FriedChickenComponent;
enum class KeyCode;
enum class JoyCode;

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

//プレイヤーと唐揚げの情報をやり取りするクラス
class PlayerChickenConnection : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ChickenPtr = std::shared_ptr<FriedChickenComponent>;

public:
    PlayerChickenConnection(std::shared_ptr<GameObject> owner);
    ~PlayerChickenConnection();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void setPlayer(const GameObject& player);
    void setChicken(const ChickenPtr& chicken);
    ChickenPtr getChicken() const;
    void playerJumpTarget(const ChickenPtr& chicken);

private:
    void setPlayerPosOnTheChicken(const FriedChickenComponent& chicken);
    void setChickenPosUnderThePlayer();
    void trackingJumpTarget();
    void rollChicken();
    void collection();

private:
    std::shared_ptr<PlayerComponent> mPlayer;
    //プレイヤーの足元の唐揚げ
    ChickenPtr mChicken;
    //プレイヤーのジャンプターゲット
    ChickenPtr mJumpTarget;

    //唐揚げのメッシュ半径
    float mChickenRadius;

    KeyCode mCollectionKey;
    JoyCode mCollectionPad;
    bool mIsJumpRoll;
};
