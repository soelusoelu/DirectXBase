#pragma once

class GameObject;
class PlayerMoveComponent;

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

//プレイヤーと唐揚げの情報をやり取りするクラス
class PlayerChickenConnection : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    PlayerChickenConnection(std::shared_ptr<GameObject> owner);
    ~PlayerChickenConnection();
    virtual void start() override;
    virtual void update() override;
    void setPlayer(const GameObjectPtr& player);
    void setChicken(const GameObjectPtr& chicken);
    GameObjectPtr getChicken() const;
    void playerJumpTarget(const GameObjectPtr& chicken);

private:
    void setPlayerPosOnTheChicken(const GameObject& chicken);
    void setChickenPosUnderThePlayer();
    void collection();

private:
    std::shared_ptr<PlayerMoveComponent> mPlayer;
    //プレイヤーの足元の唐揚げ
    GameObjectPtr mChicken;
    //プレイヤーのジャンプターゲット
    GameObjectPtr mJumpTarget;

    //プレイヤーの前フレーム位置
    Vector3 mPlayerPreviousPos;

    //唐揚げのメッシュ半径
    float mChickenRadius;
};
