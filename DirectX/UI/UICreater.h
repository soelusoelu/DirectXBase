#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/StringUtil.h"
#include <rapidjson/document.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class UIFactory;

class UICreater {
public:
    template<typename T>
    static std::shared_ptr<T> create(const std::string& type) {
        return Singleton<UIFactory>::instance().create<T>(type);
    }
};

class UI;
class Renderer;

class UIFactory {
    friend class Singleton<UIFactory>;
    using UIFunc = std::function<std::shared_ptr<UI>(std::shared_ptr<Renderer>, const rapidjson::Value&)>;

private:
    UIFactory();
    ~UIFactory();

public:
    void initialize(const std::shared_ptr<Renderer> renderer);

    template<typename T>
    std::shared_ptr<T> create(const std::string& type) {
        std::shared_ptr<T> ret = nullptr;
        auto itr = mUIFuncs.find(type);
        if (itr == mUIFuncs.end()) {
            mUIFuncs.emplace(type, &UI::create<T>);
        }
        auto ui = loadUI(type);
        ret = std::dynamic_pointer_cast<T>(ui);
        return ret;
    }

private:
    //指定のUIを読み込む
    std::shared_ptr<UI> loadUI(const std::string& type) const;
    //指定のUIの読み込み
    std::shared_ptr<UI> loadUIProperties(const rapidjson::Value& inArray, const std::string& type) const;

private:
    std::unordered_map<std::string, UIFunc> mUIFuncs;
    std::shared_ptr<Renderer> mRenderer;
    rapidjson::Document mDocument;
};
