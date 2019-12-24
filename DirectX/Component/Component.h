#pragma once

class Actor;

class Component {
protected:
    Component(Actor* owner, int updateOrder = 100);
public:
    virtual ~Component();
    //getComponent�͂����ł���
    virtual void start() = 0;
    virtual void update() = 0;
    //�I�[�i�[��Transform���X�V���ꂽ��
    virtual void onUpdateWorldTransform() {};

    int getUpdateOrder() const;
    Actor* getOwner() const;

protected:
    Actor* mOwner;
    int mUpdateOrder;
};

