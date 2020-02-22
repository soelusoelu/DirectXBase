#pragma once

#include "Math.h"
#include <functional>
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <unordered_map>

class Actor;
class Renderer;

using ActorFunc = std::function<Actor* (std::shared_ptr<Renderer>, const rapidjson::Value&)>;

class LevelLoader {
public:
    //グローバルデータを読み込む
    static bool loadGlobal(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    //アクターを読み込む
    static bool loadActors(std::shared_ptr<Renderer> renderer, const std::string& fileName);

private:
    //jsonファイルの読み込み
    static bool loadJSON(const std::string& fileName, rapidjson::Document* outDoc);
    //グローバルプロパティの読み込み
    static void loadGlobalProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inObject);
    //アクターの読み込み
    static void loadActors(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inArray);

private:
    static constexpr int LEVEL_VERSION = 1;
    static std::unordered_map<std::string, ActorFunc> mActors;
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
