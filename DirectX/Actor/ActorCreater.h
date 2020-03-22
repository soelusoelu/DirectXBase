#pragma once

#include "../Utility/Singleton.h"
#include <rapidjson/document.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

//練習がてら、ちょい楽できるように
#define ADD_COMPONENTS(className) { mComponents.emplace((#className), &Component::create<className>); }

class Actor;
class Component;
class Renderer;

class ActorCreater {
    friend class Singleton<ActorCreater>;
    using ActorFunc = std::function<std::shared_ptr<Actor>(std::shared_ptr<Renderer>, const rapidjson::Value&)>;
    using ComponentFunc = std::function<std::shared_ptr<Component>(std::shared_ptr<Actor>, const rapidjson::Value&)>;

private:
    ActorCreater();
    ~ActorCreater();

public:
    void initialize(const std::shared_ptr<Renderer> renderer);

    template<typename T>
    std::shared_ptr<T> create(const std::string& type) {
        auto itr = mCreatedActors.find(type);
        if (itr == mCreatedActors.end()) {
            mCreatedActors.emplace(type, &Actor::create<T>);
        }
        auto actor = loadActors(type);
        std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>(actor);
        return ret;
    }

private:
    //読み込み準備
    std::shared_ptr<Actor> loadActors(const std::string& type) const;
    //アクターの読み込み
    std::shared_ptr<Actor> loadActorProperties(const rapidjson::Value& inArray, const std::string& type) const;
    //コンポーネントの読み込み
    void loadComponents(std::shared_ptr<Actor> actor, const rapidjson::Value& inArray) const;

private:
    std::unordered_map<std::string, ActorFunc> mCreatedActors;
    std::unordered_map<std::string, ComponentFunc> mComponents;
    std::shared_ptr<Renderer> mRenderer;
    rapidjson::Document mDoc;
};
