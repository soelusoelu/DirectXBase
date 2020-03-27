#pragma once

#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>

class Renderer;
class Sprite;
class UIManager;

class UI : public std::enable_shared_from_this<UI> {
    using SpritePtr = std::shared_ptr<Sprite>;
    using SpritePtrList = std::list<SpritePtr>;

    enum class State {
        ACTIVE,
        NON_ACTIVE,
        CLOSING
    };

protected:
    UI(std::shared_ptr<Renderer> renderer, const std::string& type);
public:
    virtual ~UI();
    void update();
    virtual void updateUI() = 0;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {};
    void close();
    void setActive(bool value);
    bool getActive() const;
    bool isClosing() const;
    const std::string& getTypeName() const;

    //指定されたプロパティでアクターを生成
    template <typename T>
    static std::shared_ptr<UI> create(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>(renderer);
        t->addToManager();
        t->loadProperties(inObj);
        return t;
    }

    static void setUIManager(UIManager* manager);

protected:
    void addSprite(SpritePtr sprite);
    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};

private:
    void addToManager();

protected:
    std::shared_ptr<Renderer> mRenderer;

private:
    SpritePtrList mSprites;
    State mState;
    std::string mTypeName;

    static UIManager* mUIManager;
};
