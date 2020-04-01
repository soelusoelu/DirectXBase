#include "GameObjectFactory.h"
#include "GameObject.h"
#include "../Component/Camera.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/DirectionalLight.h"
#include "../Component/FriedChickenComponent.h"
#include "../Component/FriedChickenManager.h"
#include "../Component/HitPointComponent.h"
#include "../Component/JumpTarget.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/PointLightComponent.h"
#include "../Component/Score.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/TransparentMeshComponent.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"

GameObjectFactory::GameObjectFactory() :
    mRenderer(nullptr) {
    ADD_COMPONENT(Camera);
    ADD_COMPONENT(DirectionalLight);
    ADD_COMPONENT(FriedChickenComponent);
    ADD_COMPONENT(FriedChickenManager);
    ADD_COMPONENT(HitPointComponent);
    ADD_COMPONENT(JumpTarget);
    ADD_COMPONENT(MeshComponent);
    ADD_COMPONENT(PlayerMoveComponent);
    ADD_COMPONENT(PointLightComponent);
    ADD_COMPONENT(Score);
    ADD_COMPONENT(SoundComponent);
    ADD_COMPONENT(SphereCollisionComponent);
    ADD_COMPONENT(SpriteComponent);
    ADD_COMPONENT(TransparentMeshComponent);
}

GameObjectFactory::~GameObjectFactory() = default;

void GameObjectFactory::initialize(const std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;

    std::string fileName = "ActorsList.json";
    if (!Singleton<LevelLoader>::instance().loadJSON(fileName, &mDocument)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
    }
    fileName = "UIList.json";
    if (!Singleton<LevelLoader>::instance().loadJSON(fileName, &mUIDocument)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
    }
}

std::shared_ptr<GameObject> GameObjectFactory::loadGameObject(const std::string& type) const {
    GameObjectPtr obj = nullptr;
    const auto& objs = mDocument["gameObjects"];
    if (objs.IsArray()) {
        obj = loadGameObjectProperties(objs, type);
    }

    return obj;
}

std::shared_ptr<GameObject> GameObjectFactory::loadUIGameObject(const std::string& type) const {
    GameObjectPtr obj = nullptr;
    const auto& objs = mUIDocument["UIs"];
    if (objs.IsArray()) {
        obj = loadGameObjectProperties(objs, type);
    }

    return obj;
}

std::shared_ptr<GameObject> GameObjectFactory::loadGameObjectProperties(const rapidjson::Value& inArray, const std::string& type) const {
    GameObjectPtr gameObj = nullptr;
    //アクターの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const auto& obj = inArray[i];
        if (!obj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string t;
        if (!JsonHelper::getString(obj, "type", &t)) {
            continue;
        }
        //指定のタイプと一致するか
        if (t != type) {
            continue;
        }
        //mapからゲームオブジェクトを生成
        gameObj = GameObject::create(mRenderer);
        gameObj->setTag(type);
        if (obj.HasMember("properties")) {
            gameObj->loadProperties(obj["properties"]);
        }
        //コンポーネントプロパティがあれば取得
        if (!obj.HasMember("components")) {
            break;
        }
        const auto& components = obj["components"];
        if (components.IsArray()) {
            loadComponents(gameObj, components);
            break;
        }
    }

    return gameObj;
}

void GameObjectFactory::loadComponents(GameObjectPtr gameObject, const rapidjson::Value& inArray) const {
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
        itr->second(gameObject, compObj["properties"]);
    }
}

std::shared_ptr<GameObject> GameObjectCreater::create(const std::string& type) {
    return Singleton<GameObjectFactory>::instance().loadGameObject(type);
}

std::shared_ptr<GameObject> GameObjectCreater::createUI(const std::string& type) {
    return Singleton<GameObjectFactory>::instance().loadUIGameObject(type);
}
