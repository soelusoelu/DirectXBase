#pragma once

#include "Math.h"
#include "Singleton.h"
#include <functional>
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <unordered_map>

class Actor;
class Component;
class Renderer;

using ActorFunc = std::function<std::shared_ptr<Actor>(std::shared_ptr<Renderer>, const rapidjson::Value&)>;
using ComponentFunc = std::function<std::shared_ptr<Component>(std::shared_ptr<Actor>, const rapidjson::Value&)>;

class LevelLoader {
    friend class Singleton<LevelLoader>;

public:
    //グローバルデータを読み込む
    bool loadGlobal(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    //アクターを読み込む
    bool loadActors(std::shared_ptr<Renderer> renderer, const std::string& fileName);

private:
    LevelLoader();
    ~LevelLoader();

    //jsonファイルの読み込み
    bool loadJSON(const std::string& fileName, rapidjson::Document* outDoc);
    //グローバルプロパティの読み込み
    void loadGlobalProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inObject);
    //アクターの読み込み
    void loadActorsProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inArray);
    //コンポーネントの読み込み
    void loadComponents(std::shared_ptr<Actor> actor, const rapidjson::Value& inArray);

private:
    static constexpr int LEVEL_VERSION = 1;
    std::unordered_map<std::string, ActorFunc> mActors;
    std::unordered_map<std::string, ComponentFunc> mComponents;
};

class JsonHelper {
public:
    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int* out);
    static bool getFloat(const rapidjson::Value& inObject, const char* inProperty, float* out);
    static bool getString(const rapidjson::Value& inObject, const char* inProperty, std::string* out);
    static bool getBool(const rapidjson::Value& inObject, const char* inProperty, bool* out);
    static bool getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3* out);
    static bool getQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion* out);
};
