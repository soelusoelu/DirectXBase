#include "LevelLoader.h"
#include "Debug.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Actor/EmptyActor.h"
#include "../Actor/Field.h"
#include "../Actor/PlayerActor.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/HitPointComponent.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../Light/DirectionalLight.h"
#include "../System/Game.h"
#include "../UI/Score.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <fstream>
#include <vector>

LevelLoader::LevelLoader() {
    mActors = {
        { "Empty", &Actor::create<EmptyActor> },
        { "Player", &Actor::create<PlayerActor> },
        { "Field", &Actor::create<Field> }
    };

    mComponents = {
        { "HitPointComponent", &Component::create<HitPointComponent> },
        { "MeshComponent", &Component::create<MeshComponent> },
        { "PlayerMoveComponent", &Component::create<PlayerMoveComponent> },
        { "PointLightComponent", &Component::create<PointLightComponent> },
        { "SoundComponent", &Component::create<SoundComponent> },
        { "SphereCollisionComponent", &Component::create<SphereCollisionComponent> },
    };

    mUIs = {
        { "Score", &UI::create<Score> }
    };
}

LevelLoader::~LevelLoader() = default;

bool LevelLoader::loadGlobal(std::shared_ptr<Renderer> renderer, const std::string & fileName) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
        return false;
    }

    const rapidjson::Value& globals = doc["globalProperties"];
    if (globals.IsObject()) {
        loadGlobalProperties(renderer, globals);
    }

    return true;
}

bool LevelLoader::loadActors(std::shared_ptr<Renderer> renderer, const std::string & fileName) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
        return false;
    }

    const rapidjson::Value& actors = doc["actors"];
    if (actors.IsArray()) {
        loadActorsProperties(renderer, actors);
    }

    return true;
}

std::shared_ptr<Actor> LevelLoader::loadSpecifiedActor(std::shared_ptr<Renderer> renderer, const std::string & fileName, const std::string & type) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
        return nullptr;
    }

    std::shared_ptr<Actor> actor = nullptr;
    const rapidjson::Value& actors = doc["actors"];
    if (actors.IsArray()) {
        actor = loadSpecifiedActorProperties(renderer, actors, type);
    }

    return actor;
}

std::shared_ptr<UI> LevelLoader::loadSpecifiedUI(std::shared_ptr<Renderer> renderer, const std::string& fileName, const std::string& type) const {
    rapidjson::Document doc;
    if (!loadJSON(fileName, &doc)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
        return nullptr;
    }

    std::shared_ptr<UI> ui = nullptr;
    const rapidjson::Value& uis = doc["UIs"];
    if (uis.IsArray()) {
        ui = loadSpecifiedUIProperties(renderer, uis, type);
    }

    return ui;
}

void LevelLoader::saveLevel(std::shared_ptr<Renderer> renderer, const std::string & fileName) const {
    ////ドキュメントとルートオブジェクトを生成
    //rapidjson::Document doc;
    //doc.SetObject();

    //// Write the version
    ////JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

    ////グローバル
    //rapidjson::Value globals(rapidjson::kObjectType);
    //saveGlobalProperties(doc.GetAllocator(), renderer, &globals);
    //doc.AddMember("globalProperties", globals, doc.GetAllocator());

    ////アクター
    //rapidjson::Value actors(rapidjson::kArrayType);
    //saveActors(doc.GetAllocator(), renderer, &actors);
    //doc.AddMember("actors", actors, doc.GetAllocator());

    ////jsonを文字列バッファに保存
    //rapidjson::StringBuffer buffer;
    ////整形出力用にPrettyWriterを使う(もしくはWriter)
    //rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    //doc.Accept(writer);
    //const char* output = buffer.GetString();

    ////文字列をファイルに書き込む
    //std::ofstream outFile(fileName);
    //if (outFile.is_open()) {
    //    outFile << output;
    //}
}

void LevelLoader::saveUI(std::list<std::shared_ptr<UI>> uiList, const std::string& fileName) const {
    //ドキュメントとルートオブジェクトを生成
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value uis(rapidjson::kArrayType);
    for (const auto& ui : uiList) {
        //UI用のjsonオブジェクトを作る
        rapidjson::Value obj(rapidjson::kObjectType);
        //タイプを追加
        JsonHelper::setString(doc.GetAllocator(), &obj, "type", ui->getTypeName());

        //プロパティ用のjsonオブジェクトを作る
        rapidjson::Value props(rapidjson::kObjectType);
        //プロパティを保存
        ui->saveProperties(doc.GetAllocator(), &props);
        //プロパティをアクターのjsonオブジェクトに追加
        obj.AddMember("properties", props, doc.GetAllocator());

        //UIを配列に追加
        uis.PushBack(obj, doc.GetAllocator());
    }
    doc.AddMember("UIs", uis, doc.GetAllocator());

    //jsonを文字列バッファに保存
    rapidjson::StringBuffer buffer;
    //整形出力用にPrettyWriterを使う(もしくはWriter)
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    //文字列をファイルに書き込む
    setDataDirectory();
    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << output;
    }
}

bool LevelLoader::loadJSON(const std::string & fileName, rapidjson::Document * outDoc) const {
    //フォルダ階層の移動
    setDataDirectory();

    //バイナリモードで開き、末尾に移動
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Debug::windowMessage(fileName + "ファイルが見つかりません");
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
        Debug::windowMessage(fileName + "ファイルは有効ではありません");
        return false;
    }

    return true;
}

void LevelLoader::loadGlobalProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value & inObject) const {
    //環境光
    Vector3 ambient;
    if (JsonHelper::getVector3(inObject, "ambientLight", &ambient)) {
        renderer->setAmbientLight(ambient);
    }

    //ディレクショナルライト
    const rapidjson::Value& dirObj = inObject["directionalLight"];
    if (dirObj.IsObject()) {
        Vector3 dir, color;

        //向きと色を設定
        if (JsonHelper::getVector3(dirObj, "direction", &dir)) {
            renderer->getDirectionalLight()->setDirection(dir);
        }
        if (JsonHelper::getVector3(dirObj, "color", &color)) {
            renderer->getDirectionalLight()->setColor(color);
        }
    }

    //数字とフォント
    std::string num, font;
    JsonHelper::getString(inObject, "number", &num);
    JsonHelper::getString(inObject, "font", &font);
    renderer->getDrawString()->initialize(renderer, num, font);
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
            Debug::windowMessage(type + "は有効な型ではありません");
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

std::shared_ptr<Actor> LevelLoader::loadSpecifiedActorProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value & inArray, const std::string & type) const {
    //そもそも指定のtypeが存在しているかチェック
    auto itr = mActors.find(type);
    if (itr == mActors.end()) {
        Debug::windowMessage(type + "は有効な型ではありません");
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
            Debug::windowMessage(type + "は有効な型ではありません");
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

std::shared_ptr<UI> LevelLoader::loadSpecifiedUIProperties(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inArray, const std::string& type) const {
    //そもそも指定のtypeが存在しているかチェック
    auto itr = mUIs.find(type);
    if (itr == mUIs.end()) {
        Debug::windowMessage(type + "は有効な型ではありません");
        return nullptr;
    }

    std::shared_ptr<UI> ui = nullptr;
    //UIの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const rapidjson::Value& uiObj = inArray[i];
        if (!uiObj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string t;
        if (!JsonHelper::getString(uiObj, "type", &t)) {
            continue;
        }
        //指定のタイプと一致するか
        if (t != type) {
            continue;
        }
        //mapからUIを生成
        ui = itr->second(renderer, uiObj["properties"]);
    }

    return ui;
}

void LevelLoader::saveGlobalProperties(rapidjson::Document::AllocatorType & alloc, std::shared_ptr<Renderer> renderer, rapidjson::Value * inObject) const {
    //環境光
    JsonHelper::setVector3(alloc, inObject, "ambientLight", renderer->getAmbientLight());

    //ディレクショナルライト
    auto dirLight = renderer->getDirectionalLight();
    rapidjson::Value dirObj(rapidjson::kObjectType);
    JsonHelper::setVector3(alloc, &dirObj, "direction", dirLight->getDirection());
    JsonHelper::setVector3(alloc, &dirObj, "color", dirLight->getColor());
    inObject->AddMember("directionalLight", dirObj, alloc);
}

void LevelLoader::saveActors(rapidjson::Document::AllocatorType& alloc, ActorManager* manager, rapidjson::Value* inArray) const {
    //for (const auto& actor : manager->getActors()) {
    //    //アクター用のjsonオブジェクトを作る
    //    rapidjson::Value obj(rapidjson::kObjectType);
    //    //タイプを追加
    //    JsonHelper::setString(alloc, &obj, "type", actor->tag());

    //    //プロパティ用のjsonオブジェクトを作る
    //    rapidjson::Value props(rapidjson::kObjectType);
    //    //プロパティを保存
    //    actor->saveProperties(alloc, &props);
    //    //プロパティをアクターのjsonオブジェクトに追加
    //    obj.AddMember("properties", props, alloc);

    //    //コンポーネントを保存
    //    rapidjson::Value components(rapidjson::kArrayType);
    //    saveComponents(alloc, actor, &components);
    //    obj.AddMember("components", components, alloc);

    //    //アクターを配列に追加
    //    inArray->PushBack(obj, alloc);
    //}
}

void LevelLoader::saveComponents(rapidjson::Document::AllocatorType& alloc, const std::shared_ptr<Actor> actor, rapidjson::Value* inArray) const {
    //const auto& components = actor->getComponents();
    //for (const Component& comp : components) {
    //    // Make a JSON object
    //    rapidjson::Value obj(rapidjson::kObjectType);
    //    // Add type
    //    JsonHelper::setString(alloc, &obj, "type", comp->get());

    //    // Make an object for properties
    //    rapidjson::Value props(rapidjson::kObjectType);
    //    // Save rest of properties
    //    comp->SaveProperties(alloc, props);
    //    // Add the member
    //    obj.AddMember("properties", props, alloc);

    //    // Add component to array
    //    inArray.PushBack(obj, alloc);
    //}
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

bool JsonHelper::getVector2(const rapidjson::Value& inObject, const char* inProperty, Vector2* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray() || property.Size() != 2) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < 2; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());

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

void JsonHelper::setInt(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, int value) {
    rapidjson::Value v(value);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setFloat(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, float value) {
    rapidjson::Value v(value);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setString(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const std::string & value) {
    rapidjson::Value v;
    v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setBool(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, bool value) {
    rapidjson::Value v(value);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector2(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Vector2& value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, zそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector3(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const Vector3 & value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, zそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setQuaternion(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const Quaternion & value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, z, wそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}
