﻿#include "SpriteManager.h"
#include "Sprite.h"
#include "../Actor/Transform2D.h"
#include "../Device/Renderer.h"
#include "../System/BlendDesc.h"
#include "../System/BlendState.h"
#include "../System/DepthStencilState.h"
#include "../System/Game.h"
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
    Texture::vertexBuffer->setVertexBuffer();
    //インデックスバッファーをセット
    renderer->setIndexBuffer(Texture::indexBuffer);
    //デプステスト無効化
    renderer->depthStencilState()->depthTest(false);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    renderer->blendState()->setBlendState(bd);

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
