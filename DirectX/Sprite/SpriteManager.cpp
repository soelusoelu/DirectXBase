#include "SpriteManager.h"
#include "Sprite.h"
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
    remove();
}

void SpriteManager::draw(const Matrix4& proj) {
    if (mSprites.empty()) {
        return;
    }

    for (auto&& sprite : mSprites) {
        if (!sprite->getActive() || sprite->isDead()) {
            return;
        }
        sprite->draw(proj);
    }
}

void SpriteManager::add(const SpritePtr& add) {
    mSprites.emplace_back(add);
}

void SpriteManager::clear() {
    mSprites.clear();
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
}
