#include "SpriteManager.h"
#include "Sprite.h"
#include "../Component/Sprite3D.h"
#include "../Component/SpriteComponent.h"
#include "../Device/Renderer.h"
#include "../GameObject/Transform2D.h"
#include "../System/BlendDesc.h"
#include "../System/BlendState.h"
#include "../System/DepthStencilState.h"
#include "../System/DirectX.h"
#include "../System/Format.h"
#include "../System/Game.h"
#include "../System/IndexBuffer.h"
#include "../System/Texture.h"
#include "../System/VertexBuffer.h"

SpriteManager::SpriteManager() = default;

SpriteManager::~SpriteManager() = default;

void SpriteManager::update() {
    for (auto&& sprite : mSprites) {
        sprite->update();
    }
    for (auto&& sprite : mSpriteComponents) {
        sprite->update();
    }
    remove();
}

void SpriteManager::draw(const Matrix4& proj) const {
    if (mSprites.empty()) {
        return;
    }

    for (const auto& sprite : mSprites) {
        if (!sprite->getActive() || sprite->isDead()) {
            return;
        }
        sprite->draw(proj);
    }
}

void SpriteManager::drawComponents() const {
    if (mSpriteComponents.empty()) {
        return;
    }

    for (const auto& sprite : mSpriteComponents) {
        if (!sprite->getActive() || sprite->isDead()) {
            return;
        }
        sprite->draw();
    }
}

void SpriteManager::draw3Ds(const Matrix4& viewProj) const {
    if (mSprite3Ds.empty()) {
        return;
    }

    for (const auto& sprite : mSprite3Ds) {
        sprite->draw(viewProj);
    }
}

void SpriteManager::add(const SpritePtr& add) {
    mSprites.emplace_back(add);
}

void SpriteManager::addComponent(const SpriteComponentPtr& add) {
    mSpriteComponents.emplace_back(add);
}

void SpriteManager::add3D(const Sprite3DPtr& add) {
    mSprite3Ds.emplace_back(add);
}

void SpriteManager::clear() {
    mSprites.clear();
    mSpriteComponents.clear();
    mSprite3Ds.clear();
}

void SpriteManager::remove() {
    auto itr = mSprites.begin();
    while (itr != mSprites.end()) {
        if ((*itr)->isDead()) {
            itr = mSprites.erase(itr);
        } else {
            ++itr;
        }
    }

    auto itr2 = mSpriteComponents.begin();
    while (itr2 != mSpriteComponents.end()) {
        if ((*itr2)->isDead()) {
            itr2 = mSpriteComponents.erase(itr2);
        } else {
            ++itr2;
        }
    }
}
