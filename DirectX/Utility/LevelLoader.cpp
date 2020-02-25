﻿#include "LevelLoader.h"
#include "../Actor/Actor.h"
#include "../Actor/EmptyActor.h"
#include "../Actor/Field.h"
#include "../Actor/PlayerActor.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../Device/Renderer.h"
#include "../Light/DirectionalLight.h"
#include "../System/Game.h"
#include <fstream>
#include <vector>

LevelLoader::LevelLoader() {
    mActors = {
        { "Empty", &Actor::create<EmptyActor> },
        { "Player", &Actor::create<PlayerActor> },
        { "Field", &Actor::create<Field> }
    };

    mComponents = {
        { "MeshComponent", &Component::create<MeshComponent> },
        { "PlayerMoveComponent", &Component::create<PlayerMoveComponent> },
        { "PointLightComponent", &Component::create<PointLightComponent> },
        { "SoundComponent", &Component::create<SoundComponent> },
        { "SphereCollisionComponent", &Component::create<SphereCollisionComponent> },
    };
}

LevelLoader::~LevelLoader() = default;

bool LevelLoader::loadGlobal(std::shared_ptr<Renderer> renderer, const std::string & fileName) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        MSG(L"レベルファイルのロードに失敗しました");
        return false;
    }

    //int version = 0;
    //if (!JsonHelper::getInt(doc, "version", &version) || version != LEVEL_VERSION) {
    //    MSG(L"レベルファイルのバージョンが違います");
    //    return false;
    //}

    const rapidjson::Value& globals = doc["globalProperties"];
    if (globals.IsObject()) {
        loadGlobalProperties(renderer, globals);
    }

    return true;
}

bool LevelLoader::loadActors(std::shared_ptr<Renderer> renderer, const std::string & fileName) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        MSG(L"レベルファイルのロードに失敗しました");
        return false;
    }

    const rapidjson::Value& actors = doc["actors"];
    if (actors.IsArray()) {
        loadActorsProperties(renderer, actors);
    }

    return true;
}

std::shared_ptr<Actor> LevelLoader::loadSpecifiedActor(std::shared_ptr<Renderer> renderer, const std::string& fileName, const std::string& type) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        MSG(L"レベルファイルのロードに失敗しました");
        return nullptr;
    }

    std::shared_ptr<Actor> actor = nullptr;
    const rapidjson::Value& actors = doc["actors"];
    if (actors.IsArray()) {
        actor = loadSpecifiedActorProperties(renderer, actors, type);
    }

    return actor;
}

bool LevelLoader::loadJSON(const std::string & fileName, rapidjson::Document * outDoc) const {
    //フォルダ階層の移動
    setDataDirectory();

    //バイナリモードで開き、末尾に移動
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        MSG(L"jsonファイルが見つかりません");
        return false;
    }

    //ファイルの末尾 = ファイルサイズ
    std::ifstream::pos_type fileSize = file.tellg();
    //ファイルの先頭まで戻る
    file.seekg(0, std::ios::beg);

    //末尾のヌルを含めたsize+1の配列を作る
    std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
    //配列にバイト列を書き込む
    file.read(bytes.data(), static_cast<size_t>(fileSize));

    //生データをRapidJSONドキュメントにロードする
    outDoc->Parse(bytes.data());
    if (!outDoc->IsObject()) {
        MSG(L"有効なjsonファイルではありません");
        return false;
    }

    return true;
}

void LevelLoader::loadGlobalProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value & inObject) const {
    //環境光を取得
    Vector3 ambient;
    if (JsonHelper::getVector3(inObject, "ambientLight", &ambient)) {
        renderer->setAmbientLight(ambient);
    }

    //ディレクショナルライトを取得
    const rapidjson::Value& dirObj = inObject["directionalLight"];
    if (dirObj.IsObject()) {
        //向きと色を設定
        JsonHelper::getVector3(dirObj, "direction", &DirectionalLight::direction);
        JsonHelper::getVector3(dirObj, "color", &DirectionalLight::color);
    }
}

void LevelLoader::loadActorsProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value & inArray) const {
    //アクターの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const rapidjson::Value& actorObj = inArray[i];
        if (!actorObj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string type;
        if (!JsonHelper::getString(actorObj, "type", &type)) {
            continue;
        }
        //mapに存在するか
        auto iter = mActors.find(type);
        if (iter == mActors.end()) {
            continue;
        }
        //mapからアクターを生成
        auto actor = iter->second(renderer, actorObj["properties"]);
        //コンポーネントプロパティがあれば取得
        if (!actorObj.HasMember("components")) {
            continue;
        }
        const rapidjson::Value& components = actorObj["components"];
        if (components.IsArray()) {
            loadComponents(actor, components);
        }
    }
}

std::shared_ptr<Actor> LevelLoader::loadSpecifiedActorProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inArray, const std::string& type) const {
    //そもそも指定のtypeが存在しているかチェック
    auto itr = mActors.find(type);
    if (itr == mActors.end()) {
        return nullptr;
    }

    std::shared_ptr<Actor> actor = nullptr;
    //アクターの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const rapidjson::Value& actorObj = inArray[i];
        if (!actorObj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string t;
        if (!JsonHelper::getString(actorObj, "type", &t)) {
            continue;
        }
        //指定のタイプと一致するか
        if (t != type) {
            continue;
        }
        //mapからアクターを生成
        actor = itr->second(renderer, actorObj["properties"]);
        //コンポーネントプロパティがあれば取得
        if (!actorObj.HasMember("components")) {
            break;
        }
        const rapidjson::Value& components = actorObj["components"];
        if (components.IsArray()) {
            loadComponents(actor, components);
            break;
        }
    }

    return actor;
}

void LevelLoader::loadComponents(std::shared_ptr<Actor> actor, const rapidjson::Value & inArray) const {
    //コンポーネントの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const rapidjson::Value& compObj = inArray[i];
        if (!compObj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string type;
        if (!JsonHelper::getString(compObj, "type", &type)) {
            continue;
        }
        //mapに存在するか
        auto iter = mComponents.find(type);
        if (iter == mComponents.end()) {
            continue;
        }
        //アクターがそのコンポーネントを保持しているか
        auto comp = actor->componentManager()->getComponent(iter->first);
        if (comp) {
            //プロパティをロード
            comp->loadProperties(compObj["properties"]);
        } else {
            //新規コンポーネントを生成
            comp = iter->second(actor, compObj["properties"]);
        }
    }
}

bool JsonHelper::getInt(const rapidjson::Value & inObject, const char* inProperty, int* out) {
    //プロパティが存在するか
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    //値の方を取得し、整数であることを確認
    auto& property = itr->value;
    if (!property.IsInt()) {
        return false;
    }

    //プロパティの取得に成功
    *out = property.GetInt();
    return true;
}

bool JsonHelper::getFloat(const rapidjson::Value & inObject, const char* inProperty, float* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsDouble()) {
        return false;
    }

    *out = static_cast<float>(property.GetDouble());
    return true;
}

bool JsonHelper::getString(const rapidjson::Value & inObject, const char* inProperty, std::string * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsString()) {
        return false;
    }

    *out = property.GetString();
    return true;
}

bool JsonHelper::getBool(const rapidjson::Value & inObject, const char* inProperty, bool* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsBool()) {
        return false;
    }

    *out = property.GetBool();
    return true;
}

bool JsonHelper::getVector3(const rapidjson::Value & inObject, const char* inProperty, Vector3 * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray() || property.Size() != 3) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < 3; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());
    out->z = static_cast<float>(property[2].GetDouble());

    return true;
}

bool JsonHelper::getQuaternion(const rapidjson::Value & inObject, const char* inProperty, Quaternion * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;

    for (rapidjson::SizeType i = 0; i < 4; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());
    out->z = static_cast<float>(property[2].GetDouble());
    out->w = static_cast<float>(property[3].GetDouble());

    return true;
}
