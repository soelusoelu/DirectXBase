#pragma once

#include <list>
#include <memory>

enum class UIState {
    ACTIVE,
    CLOSING
};

class UIManager;
class Sprite;

class UI {
protected:
    UI();
public:
    virtual ~UI();
    void update();
    virtual void updateUI() = 0;
    void close();
    void addSprite(Sprite* sprite);
    void removeSprite(Sprite* sprite);
    UIState getState() const;

    static void setUIManager(UIManager* manager);

private:
    std::list<Sprite*> mSprites;
    UIState mState;

    static UIManager* mUIManager;
};
