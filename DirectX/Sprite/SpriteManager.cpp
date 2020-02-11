#include "SpriteManager.h"
#include "Sprite.h"
#include "Texture.h"
#include "../Actor/Transform2D.h"
#include "../Device/Renderer.h"
#include "../System/Game.h"
#include "../System/VertexStreamDesc.h"
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
    if (mSprites.empty()) {
        return;
    }
    //スプライト共通作業
    //プロジェクション
    Matrix4 proj = Matrix4::identity;
    //原点をスクリーン左上にするために平行移動
    proj.m[3][0] = -1.f;
    proj.m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj.m[0][0] = 2.f / Game::WINDOW_WIDTH;
    proj.m[1][1] = -2.f / Game::WINDOW_HEIGHT;

    //プリミティブ・トポロジーをセット
    renderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    VertexStreamDesc stream;
    stream.buffer = Texture::vertexBuffer;
    stream.offset = 0;
    stream.stride = sizeof(TextureVertex);
    renderer->setVertexBuffer(&stream);
    //インデックスバッファーをセット
    renderer->setIndexBuffer(Texture::indexBuffer);

    for (const auto& sprite : mSprites) {
        sprite->draw(proj);
    }
}

void SpriteManager::add(Sprite * add) {
    std::shared_ptr<Sprite> s(add);
    this->add(s);
}

void SpriteManager::add(std::shared_ptr<Sprite> add) {
    float z = add->transform()->getDepth();
    auto itr = mSprites.begin();
    for (; itr != mSprites.end(); ++itr) {
        if (z > (*itr)->transform()->getDepth()) {
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
        if ((*itr)->isDead()) {
            itr = mSprites.erase(itr);
        } else {
            ++itr;
        }
    }
}

void SpriteManager::sortByZ() {
    if (Transform2D::zSortFlag) { //z値を変更したやつがいればソート
        Transform2D::zSortFlag = false;
        std::sort(mSprites.begin(), mSprites.end(), [](std::shared_ptr<Sprite> a, std::shared_ptr<Sprite> b) {
            return a->transform()->getDepth() > b->transform()->getDepth();
        });
    }
}
