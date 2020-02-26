#include "SpriteManager.h"
#include "Sprite.h"
#include "../Actor/Transform2D.h"
#include "../Device/Renderer.h"
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

void SpriteManager::draw() {
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
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    Texture::vertexBuffer->setVertexBuffer();
    //インデックスバッファーをセット
    Texture::indexBuffer->setIndexBuffer(Format::FORMAT_R16_UINT);
    //デプステスト無効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(false);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    Singleton<DirectX>::instance().blendState()->setBlendState(bd);

    for (auto&& sprite : mSprites) {
        if (!sprite->getActive() || sprite->isDead()) {
            return;
        }
        sprite->draw(proj);
    }
}

void SpriteManager::add(SpritePtr add) {
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
