#pragma once

#include <vector>

class Actor;
class Collider;

class Physics {
public:
    Physics();
    ~Physics();
    //ボックスコンポーネントの追加・削除
    void add(Collider* circle);
    void remove(Collider* circle);
    //全削除
    void clear();
    //総当たり判定
    void sweepAndPrune();

private:
    std::vector<Collider*> mColliders;
};
