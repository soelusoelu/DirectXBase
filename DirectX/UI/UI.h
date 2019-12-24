#pragma once

#include <list>
#include <memory>

enum class UIState {
    ACTIVE,
    CLOSING
};

class Sprite;
class UIManager;

class UI {
protected:
    UI();
public:
    virtual ~UI();
    virtual void update() = 0;
    void close();
    void addSprite(std::shared_ptr<Sprite> add);
    UIState getState() const;

    static void setUIManager(UIManager* manager);

private:
    //std::list<std::shared_ptr<Sprite>> mSprites;
    UIState mState;

    static UIManager* mUIManager;
};
