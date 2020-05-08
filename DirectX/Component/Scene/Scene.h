#pragma once

#include "../Component.h"
#include <string>

class Scene : public Component {
public:
    Scene(const std::shared_ptr<GameObject>& owner);
    ~Scene();
    void next(const std::string& next);
    const std::string& getNext() const;

private:
    std::string mNext;
};
