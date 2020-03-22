#include "ActorCreater.h"
#include "Actor.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/HitPointComponent.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"

ActorCreater::ActorCreater() :
    mRenderer(nullptr) {
    ADD_COMPONENTS(HitPointComponent);
    ADD_COMPONENTS(MeshComponent);
    ADD_COMPONENTS(PlayerMoveComponent);
    ADD_COMPONENTS(PointLightComponent);
    ADD_COMPONENTS(SoundComponent);
    ADD_COMPONENTS(SphereCollisionComponent);
}

ActorCreater::~ActorCreater() = default;

void ActorCreater::initialize(const std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;

    const std::string fileName = "ActorsList.json";
    if (!Singleton<LevelLoader>::instance().loadJSON(fileName, &mDoc)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
    }
}

std::shared_ptr<Actor> ActorCreater::loadActors(const std::string& type) const {
    std::shared_ptr<Actor> actor = nullptr;
    const auto& actors = mDoc["actors"];
    if (actors.IsArray()) {
        actor = loadActorProperties(actors, type);
    }

    return actor;
}

std::shared_ptr<Actor> ActorCreater::loadActorProperties(const rapidjson::Value & inArray, const std::string & type) const {
    std::shared_ptr<Actor> actor = nullptr;
    //アクターの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const auto& actorObj = inArray[i];
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
        actor = mCreatedActors.at(type)(mRenderer, actorObj["properties"]);
        //コンポーネントプロパティがあれば取得
        if (!actorObj.HasMember("components")) {
            break;
        }
        const auto& components = actorObj["components"];
        if (components.IsArray()) {
            loadComponents(actor, components);
            break;
        }
    }

    return actor;
}

void ActorCreater::loadComponents(std::shared_ptr<Actor> actor, const rapidjson::Value& inArray) const {
    //コンポーネントの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const auto& compObj = inArray[i];
        if (!compObj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string type;
        if (!JsonHelper::getString(compObj, "type", &type)) {
            continue;
        }
        //mapに存在するか
        auto itr = mComponents.find(type);
        if (itr == mComponents.end()) {
            Debug::windowMessage(type + "は有効な型ではありません");
            continue;
        }
        //アクターがそのコンポーネントを保持しているか
        auto comp = actor->componentManager()->getComponent(itr->first);
        if (comp) {
            //プロパティをロード
            comp->loadProperties(compObj["properties"]);
        } else {
            //新規コンポーネントを生成
            comp = itr->second(actor, compObj["properties"]);
        }
    }
}
