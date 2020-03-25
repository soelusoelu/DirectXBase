#pragma once

#include <memory>

class FriedChicken;
class PlayerActor;

//プレイヤーと唐揚げの情報をやり取りするクラス
class PlayerChickenConnection {
    using PlayerPtr = std::shared_ptr<PlayerActor>;
    using ChickenPtr = std::shared_ptr<FriedChicken>;

public:
    PlayerChickenConnection();
    ~PlayerChickenConnection();
    void initialize();
    void connect();
    void setPlayer(const PlayerPtr player);
    void setChicken(const ChickenPtr chicken);
    ChickenPtr getChicken() const;
    void playerJumpTarget(const ChickenPtr chicken);

private:
    PlayerPtr mPlayer;
    //プレイヤーの足元の唐揚げ
    ChickenPtr mChicken;
    //プレイヤーのジャンプターゲット
    ChickenPtr mJumpTarget;

    //プレイヤーと唐揚げのメッシュ半径
    float mPlayerRadius;
    float mChickenRadius;
};
