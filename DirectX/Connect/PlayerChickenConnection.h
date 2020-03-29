#pragma once

class Actor;
class PlayerMoveComponent;

#include <memory>

//プレイヤーと唐揚げの情報をやり取りするクラス
class PlayerChickenConnection {
    using ActorPtr = std::shared_ptr<Actor>;

public:
    PlayerChickenConnection();
    ~PlayerChickenConnection();
    void initialize();
    void connect();
    void setPlayer(const ActorPtr player);
    void setChicken(const ActorPtr chicken);
    ActorPtr getChicken() const;
    void playerJumpTarget(const ActorPtr chicken);

private:
    std::shared_ptr<PlayerMoveComponent> mPlayer;
    //プレイヤーの足元の唐揚げ
    ActorPtr mChicken;
    //プレイヤーのジャンプターゲット
    ActorPtr mJumpTarget;

    //唐揚げのメッシュ半径
    float mChickenRadius;
};
