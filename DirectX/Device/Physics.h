#pragma once

#include <memory>
#include <vector>

class Actor;
class Collider;

class Physics {
    using CollPtr = std::shared_ptr<Collider>;
    using CollPtrArray = std::vector<CollPtr>;

public:
    Physics();
    ~Physics();
    //ボックスコンポーネントの追加・削除
    void add(const CollPtr& circle);
    void remove(const CollPtr& circle);
    //全削除
    void clear();
    //総当たり判定
    void sweepAndPrune();

private:
    CollPtrArray mColliders;
};
