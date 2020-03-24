#pragma once

#include <memory>

class FriedChicken;
class PlayerActor;

//プレイヤーと唐揚げの情報をやり取りするクラス
class PlayerChickenConnection {
public:
    PlayerChickenConnection();
    ~PlayerChickenConnection();
    void connect();
    void setPlayer(const std::shared_ptr<PlayerActor> player);
    void setChicken(const std::shared_ptr<FriedChicken> chicken);
    std::shared_ptr<FriedChicken> getChicken() const;
    void playerJumpTarget(const std::shared_ptr<FriedChicken> chicken);

private:
    std::shared_ptr<PlayerActor> mPlayer;
    //プレイヤーの足元の唐揚げ
    std::shared_ptr<FriedChicken> mChicken;
    //プレイヤーのジャンプターゲット
    std::shared_ptr<FriedChicken> mJumpTarget;
};
