#include "ActorCreater.h"
#include "Actor.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/FriedChickenComponent.h"
#include "../Component/HitPointComponent.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"

ActorFactory::ActorFactory() :
    mRenderer(nullptr) {
    ADD_COMPONENT(FriedChickenComponent);
    ADD_COMPONENT(HitPointComponent);
    ADD_COMPONENT(MeshComponent);
    ADD_COMPONENT(PlayerMoveComponent);
    ADD_COMPONENT(PointLightComponent);
    ADD_COMPONENT(SoundComponent);
    ADD_COMPONENT(SphereCollisionComponent);
}

ActorFactory::~ActorFactory() = default;

void ActorFactory::initialize(const std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;

    const std::string fileName = "ActorsList.json";
    if (!Singleton<LevelLoader>::instance().loadJSON(fileName, &mDocument)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
    }
}

std::shared_ptr<Actor> ActorFactory::loadActors(const std::string & type) const {
    std::shared_ptr<Actor> actor = nullptr;
    const auto& actors = mDocument["actors"];
    if (actors.IsArray()) {
        actor = loadActorProperties(actors, type);
    }

    return actor;
}

std::shared_ptr<Actor> ActorFactory::loadActorProperties(const rapidjson::Value & inArray, const std::string & type) const {
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
        actor = Actor::create(mRenderer, type, actorObj["properties"]);
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

void ActorFactory::loadComponents(std::shared_ptr<Actor> actor, const rapidjson::Value & inArray) const {
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
        //新規コンポーネントを生成
        itr->second(actor, compObj["properties"]);
    }
}
