#pragma once

#include "../Utility/Singleton.h"
#include <memory>
#include <string>
#include <unordered_map>

class Actor;
class Renderer;

class ActorCreater {
    friend class Singleton<ActorCreater>;

private:
    ActorCreater();
    ~ActorCreater();

public:
    void initialize(const std::shared_ptr<Renderer> renderer);

    template<typename T>
    std::shared_ptr<T> create(const std::string& type) {
        auto actor = create(type);
        std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>(actor);
        return ret;
    }

private:
    std::shared_ptr<Actor> create(const std::string& type);

private:
    std::unordered_map<std::string, std::shared_ptr<Actor>> mCreatedActors;
    std::shared_ptr<Renderer> mRenderer;
};
