#include "SpriteManager.h"
#include "Sprite.h"
#include "Texture.h"
#include <algorithm>

SpriteManager::SpriteManager() :
    mSprites(0) {
}

SpriteManager::~SpriteManager() = default;

void SpriteManager::update() {
    for (auto&& sprite : mSprites) {
        sprite->update();
    }
    remove();
    sortByZ();
}

void SpriteManager::draw(std::shared_ptr<Renderer> renderer) {
    Texture::drawAll(mSprites, renderer);
}

void SpriteManager::add(Sprite* add) {
    std::shared_ptr<Sprite> s(add);
    this->add(s);
}

void SpriteManager::add(std::shared_ptr<Sprite> add) {
    float z = add->getDepth();
    auto itr = mSprites.begin();
    for (; itr != mSprites.end(); ++itr) {
        if (z > (*itr)->getDepth()) {
            break;
        }
    }
    mSprites.insert(itr, add);
}

void SpriteManager::clear() {
    mSprites.clear();
}

void SpriteManager::remove() {
    auto itr = mSprites.begin();
    while (itr != mSprites.end()) {
        if ((*itr)->getState() == SpriteState::DEAD) {
            itr = mSprites.erase(itr);
        } else {
            ++itr;
        }
    }
}

void SpriteManager::sortByZ() {
    if (Sprite::ZSortFlag) { //z値を変更したやつがいればソート
        Sprite::ZSortFlag = false;
        //std::sort(mSprites.begin(), mSprites.end(), [](std::shared_ptr<Sprite> a, std::shared_ptr<Sprite> b) {
        //    return a->getDepth() > b->getDepth();
        //});
    }
}
