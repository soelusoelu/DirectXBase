#pragma once

#include "Math.h"
#include "Singleton.h"
#include <functional>
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <unordered_map>

class Actor;
class ActorManager;
class Component;
class Renderer;

class LevelLoader {
    friend class Singleton<LevelLoader>;

    using ActorFunc = std::function<std::shared_ptr<Actor>(std::shared_ptr<Renderer>, const rapidjson::Value&)>;
    using ComponentFunc = std::function<std::shared_ptr<Component>(std::shared_ptr<Actor>, const rapidjson::Value&)>;

public:
    //グローバルデータを読み込む
    bool loadGlobal(std::shared_ptr<Renderer> renderer, const std::string& fileName) const;
    //アクターを読み込む
    bool loadActors(std::shared_ptr<Renderer> renderer, const std::string& fileName) const;
    //指定のアクターを読み込む
    std::shared_ptr<Actor> loadSpecifiedActor(std::shared_ptr<Renderer> renderer, const std::string& fileName, const std::string& type) const;
    //保存
    void saveLevel(std::shared_ptr<Renderer> renderer, const std::string& fileName) const;

private:
    LevelLoader();
    ~LevelLoader();

    //jsonファイルの読み込み
    bool loadJSON(const std::string& fileName, rapidjson::Document* outDoc) const;
    //グローバルプロパティの読み込み
    void loadGlobalProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inObject) const;
    //アクターの読み込み
    void loadActorsProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inArray) const;
    //指定のアクターの読み込み
    std::shared_ptr<Actor> loadSpecifiedActorProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inArray, const std::string& type) const;
    //コンポーネントの読み込み
    void loadComponents(std::shared_ptr<Actor> actor, const rapidjson::Value& inArray) const;

    //グローバルプロパティの保存
    void saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, std::shared_ptr<Renderer> renderer, rapidjson::Value* inObject) const;
    //アクターの保存
    void saveActors(rapidjson::Document::AllocatorType& alloc, ActorManager* manager, rapidjson::Value* inArray) const;
    //コンポーネントの保存
    void saveComponents(rapidjson::Document::AllocatorType& alloc, const std::shared_ptr<Actor> actor, rapidjson::Value* inArray) const;

private:
    static constexpr int LEVEL_VERSION = 1;
    std::unordered_map<std::string, ActorFunc> mActors;
    std::unordered_map<std::string, ComponentFunc> mComponents;
};

class JsonHelper {
public:
    //解析に成功したらtrue, 失敗したらfalse
    //true: プロパティの名前から値をセット
    //false: 値を変えずにreturn
    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int* out);
    static bool getFloat(const rapidjson::Value& inObject, const char* inProperty, float* out);
    static bool getString(const rapidjson::Value& inObject, const char* inProperty, std::string* out);
    static bool getBool(const rapidjson::Value& inObject, const char* inProperty, bool* out);
    static bool getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3* out);
    static bool getQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion* out);

    static void setInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, int value);
    static void setFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, float value);
    static void setString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const std::string& value);
    static void setBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, bool value);
    static void setVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Vector3& value);
    static void setQuaternion(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Quaternion& value);
};
