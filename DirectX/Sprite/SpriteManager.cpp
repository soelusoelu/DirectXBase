#include "SpriteManager.h"
#include "Sprite.h"
#include "Texture.h"
#include "../Actor/Transform2D.h"
#include "../Device/Renderer.h"
#include "../System/Game.h"
#include "../System/VertexStreamDesc.h"
#include <algorithm>

SpriteManager::SpriteManager() = default;

SpriteManager::~SpriteManager() = default;

void SpriteManager::update() {
    for (auto&& sprite : mSprites) {
        sprite->update();
    }
    remove();
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
    //デプステスト無効化
    renderer->disabledDepthTest();

    for (const auto& sprite : mSprites) {
        sprite->draw(proj);
    }
}

void SpriteManager::add(Sprite * add) {
    std::shared_ptr<Sprite> s(add);
    this->add(s);
}

void SpriteManager::add(std::shared_ptr<Sprite> add) {
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
