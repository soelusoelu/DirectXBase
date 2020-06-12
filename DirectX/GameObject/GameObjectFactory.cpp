#include "GameObjectFactory.h"
#include "GameObject.h"
#include "../Component/Bird.h"
#include "../Component/BirdOrbit.h"
#include "../Component/Camera.h"
#include "../Component/ChickenCollection.h"
#include "../Component/ChickenColorChanger.h"
#include "../Component/ChickenExclamation.h"
#include "../Component/ChickenFry.h"
#include "../Component/ChickenMeshComponent.h"
#include "../Component/ChickenRise.h"
#include "../Component/CircleCollision.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/DirectionalLight.h"
#include "../Component/Fade.h"
#include "../Component/Field.h"
#include "../Component/FriedChickenComponent.h"
#include "../Component/FriedChickenManager.h"
#include "../Component/HitPointComponent.h"
#include "../Component/JumpTarget.h"
#include "../Component/MeshComponent.h"
#include "../Component/Oil.h"
#include "../Component/OneRemain.h"
#include "../Component/PlayerChickenConnection.h"
#include "../Component/PlayerComponent.h"
#include "../Component/PlayerJump.h"
#include "../Component/PlayerWalk.h"
#include "../Component/PointLightComponent.h"
#include "../Component/ResultChicken.h"
#include "../Component/ResultChickenFall.h"
#include "../Component/ResultChickenManager.h"
#include "../Component/ResultRank.h"
#include "../Component/Score.h"
#include "../Component/ScoreEvaluation.h"
#include "../Component/SoundComponent.h"
#include "../Component/SphereCollisionComponent.h"
#include "../Component/Sprite3D.h"
#include "../Component/SpriteComponent.h"
#include "../Component/Text.h"
#include "../Component/TextFloat.h"
#include "../Component/TextNumber.h"
#include "../Component/TimeLimit.h"
#include "../Component/TransparentMeshComponent.h"
#include "../Component/Scene/GamePlay.h"
#include "../Component/Scene/OperationExplanation.h"
#include "../Component/Scene/ResultScene.h"
#include "../Component/Scene/Scene.h"
#include "../Component/Scene/Title.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"

GameObjectFactory::GameObjectFactory() :
    mRenderer(nullptr) {
    ADD_COMPONENT(Bird);
    ADD_COMPONENT(BirdOrbit);
    ADD_COMPONENT(Camera);
    ADD_COMPONENT(ChickenCollection);
    ADD_COMPONENT(ChickenColorChanger);
    ADD_COMPONENT(ChickenExclamation);
    ADD_COMPONENT(ChickenFry);
    ADD_COMPONENT(ChickenMeshComponent);
    ADD_COMPONENT(ChickenRise);
    ADD_COMPONENT(CircleCollision);
    ADD_COMPONENT(DirectionalLight);
    ADD_COMPONENT(Fade);
    ADD_COMPONENT(Field);
    ADD_COMPONENT(FriedChickenComponent);
    ADD_COMPONENT(FriedChickenManager);
    ADD_COMPONENT(HitPointComponent);
    ADD_COMPONENT(JumpTarget);
    ADD_COMPONENT(MeshComponent);
    ADD_COMPONENT(Oil);
    ADD_COMPONENT(OneRemain);
    ADD_COMPONENT(PlayerChickenConnection);
    ADD_COMPONENT(PlayerComponent);
    ADD_COMPONENT(PlayerJump);
    ADD_COMPONENT(PlayerWalk);
    ADD_COMPONENT(PointLightComponent);
    ADD_COMPONENT(ResultChicken);
    ADD_COMPONENT(ResultChickenFall);
    ADD_COMPONENT(ResultChickenManager);
    ADD_COMPONENT(ResultRank);
    ADD_COMPONENT(Score);
    ADD_COMPONENT(ScoreEvaluation);
    ADD_COMPONENT(SoundComponent);
    ADD_COMPONENT(SphereCollisionComponent);
    ADD_COMPONENT(Sprite3D);
    ADD_COMPONENT(SpriteComponent);
    ADD_COMPONENT(Text);
    ADD_COMPONENT(TextFloat);
    ADD_COMPONENT(TextNumber);
    ADD_COMPONENT(TimeLimit);
    ADD_COMPONENT(TransparentMeshComponent);
    ADD_COMPONENT(GamePlay);
    ADD_COMPONENT(OperationExplanation);
    ADD_COMPONENT(ResultScene);
    ADD_COMPONENT(Scene);
    ADD_COMPONENT(Title);
}

GameObjectFactory::~GameObjectFactory() = default;

void GameObjectFactory::initialize(const std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;

    const std::string& fileName = "ActorsList.json";
    if (!Singleton<LevelLoader>::instance().loadJSON(fileName, &mDocument)) {
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

void GameObjectFactory::loadComponents(const GameObjectPtr& gameObject, const rapidjson::Value& inArray) const {
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
        itr->second(gameObject, type, compObj["properties"]);
    }
}

std::shared_ptr<GameObject> GameObjectCreater::create(const std::string& type) {
    return Singleton<GameObjectFactory>::instance().loadGameObject(type);
}
