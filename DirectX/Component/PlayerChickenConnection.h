#pragma once

#include "Component.h"
#include "../Input/Input.h"
#include "../Math/Math.h"
#include <functional>
#include <memory>

class GameObject;
class PlayerComponent;
class FriedChickenComponent;
class SoundComponent;
class Time;
class Subject;

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
    //プレイヤーのセット
    void setPlayer(const GameObject& player);
    //プレイヤーの足元の唐揚げのセット
    void setChicken(const ChickenPtr& chicken);
    const ChickenPtr& getChicken() const;
    //ジャンプターゲットが有効か
    bool isJumpTarget() const;
    //ジャンプターゲットのトップポジション
    Vector3 getJumpTargetTopPos() const;
    void setPlayerJumpTarget(const ChickenPtr& chicken);
    //回収条件を満たしていない
    void onFailedCollection(const std::function<void()>& f);

private:
    //プレイヤーの位置を引数の唐揚げの上に設定
    void setPlayerPosOnTheChicken(const FriedChickenComponent& chicken);
    //唐揚げの位置をプレイヤーの下に設定
    void setChickenPosUnderThePlayer();
    //ジャンプする唐揚げを追いかける
    void trackingJumpTarget();
    //プレイヤーの移動量に応じて唐揚げを回転させる
    void rollChicken();
    //プレイヤーの位置をジャンプする唐揚げの上に設定
    void setPlayerPosOnTheJumpTarget();
    //プレイヤーの足元の唐揚げを回収する
    void collection();
    //プレイヤーの足元の唐揚げが焦げたときの処理
    void tooBurntUnderThePlayer();
    //唐揚げが鳥に喰われたときの処理
    void eatenChicken();

private:
    std::shared_ptr<PlayerComponent> mPlayer;
    //プレイヤーの足元の唐揚げ
    ChickenPtr mChicken;
    //プレイヤーのジャンプターゲット
    ChickenPtr mJumpTarget;

    //唐揚げのメッシュ半径
    float mChickenRadius;

    std::shared_ptr<SoundComponent> mSound;
    std::unique_ptr<Time> mReplaySoundTimer;

    //回収失敗のサブジェクト
    std::unique_ptr<Subject> mFailedCollectionSubject;

    KeyCode mCollectionKey;
    JoyCode mCollectionPad;
    bool mIsJumpRoll;
};
