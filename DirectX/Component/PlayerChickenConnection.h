#pragma once

class GameObject;
class PlayerMoveComponent;
class FriedChickenComponent;

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
    void setPlayer(const GameObjectPtr& player);
    void setChicken(const ChickenPtr& chicken);
    ChickenPtr getChicken() const;
    void playerJumpTarget(const ChickenPtr& chicken);

private:
    void setPlayerPosOnTheChicken(const FriedChickenComponent& chicken);
    void setChickenPosUnderThePlayer();
    void collection();

private:
    std::shared_ptr<PlayerMoveComponent> mPlayer;
    //プレイヤーの足元の唐揚げ
    ChickenPtr mChicken;
    //プレイヤーのジャンプターゲット
    ChickenPtr mJumpTarget;

    //唐揚げのメッシュ半径
    float mChickenRadius;
};
