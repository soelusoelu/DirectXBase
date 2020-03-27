#include "UICreater.h"
#include "Score.h"
#include "UI.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/LevelLoader.h"

UIFactory::UIFactory() :
    mRenderer(nullptr) {
}

UIFactory::~UIFactory() = default;

void UIFactory::initialize(const std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;

    const std::string fileName = "UIList.json";
    if (!Singleton<LevelLoader>::instance().loadJSON(fileName, &mDocument)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
    }
}

std::shared_ptr<UI> UIFactory::loadUI(const std::string & type) const {
    std::shared_ptr<UI> ui = nullptr;
    const auto& uis = mDocument["UIs"];
    if (uis.IsArray()) {
        ui = loadUIProperties(uis, type);
    }

    return ui;
}

std::shared_ptr<UI> UIFactory::loadUIProperties(const rapidjson::Value & inArray, const std::string & type) const {
    std::shared_ptr<UI> ui = nullptr;
    //UIの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const auto& uiObj = inArray[i];
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
        ui = mUIFuncs.at(type)(mRenderer, uiObj["properties"]);
        break;
    }

    return ui;
}
