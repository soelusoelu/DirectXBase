#pragma once

#include "../Utility/Singleton.h"
#include <rapidjson/document.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

//練習がてら、ちょい楽できるように
#define ADD_COMPONENT(className) { mComponents.emplace((#className), &Component::create<className>); }

class GameObject;
class Component;
class Renderer;

class GameObjectFactory {
    friend class Singleton<GameObjectFactory>;
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ComponentFunc = std::function<void(const GameObjectPtr&, const std::string&, const rapidjson::Value&)>;

private:
    GameObjectFactory();
    ~GameObjectFactory();

public:
    void initialize(const std::shared_ptr<Renderer> renderer);
    //ファイルからゲームオブジェクト生成
    GameObjectPtr loadGameObject(const std::string& type) const;

private:
    //アクターの読み込み
    GameObjectPtr loadGameObjectProperties(const rapidjson::Value& inArray, const std::string& type) const;
    //コンポーネントの読み込み
    void loadComponents(const GameObjectPtr& gameObject, const rapidjson::Value& inArray) const;

private:
    std::unordered_map<std::string, ComponentFunc> mComponents;
    std::shared_ptr<Renderer> mRenderer;
    rapidjson::Document mDocument;
};

class GameObjectCreater {
public:
    GameObjectCreater() = delete;
    ~GameObjectCreater() = delete;

    static std::shared_ptr<GameObject> create(const std::string& type);
};
