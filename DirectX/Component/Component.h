#pragma once

class Actor;

class Component {
protected:
    Component(Actor* owner, int updateOrder = 100);
public:
    virtual ~Component();
    //getComponentはここでして
    virtual void start() = 0;
    virtual void update() = 0;
    //オーナーのTransformが更新されたら
    virtual void onUpdateWorldTransform() {};

    int getUpdateOrder() const;
    Actor* getOwner() const;

protected:
    Actor* mOwner;
    int mUpdateOrder;
};

