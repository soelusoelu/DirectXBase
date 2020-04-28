#include "Billboard.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Sprite/Sprite.h"
#include "../System/BlendDesc.h"
#include "../System/BlendState.h"
#include "../System/DepthStencilState.h"
#include "../System/DirectX.h"
#include "../System/Format.h"
#include "../System/IndexBuffer.h"
#include "../System/Texture.h"
#include "../System/VertexBuffer.h"
#include "../System/Window.h"
#include "../GameObject/Transform2D.h"
#include "../GameObject/Transform3D.h"

Billboard::Billboard(std::shared_ptr<GameObject> owner) :
    Component(owner, "Billboard") {
}

Billboard::~Billboard() = default;

//void Billboard::start() {
//    auto camera = owner()->getGameObjectManager()->find("Camera");
//    mCamera = camera->componentManager()->getComponent<Camera>();
//    mTransform = std::make_shared<Transform3D>(owner());
//    mSprite->transform()->setScale(300.f);
//    mSprite->transform()->translate(Vector2(300.f, 0.f));
//}
//
//void Billboard::draw() const {
//    //プリミティブ・トポロジーをセット
//    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
//    //バーテックスバッファーをセット
//    Texture::vertexBuffer->setVertexBuffer();
//    //インデックスバッファーをセット
//    Texture::indexBuffer->setIndexBuffer(Format::FORMAT_R16_UINT);
//    //デプステスト無効化
//    Singleton<DirectX>::instance().depthStencilState()->depthTest(false);
//    //通常合成
//    BlendDesc bd;
//    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
//    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
//    Singleton<DirectX>::instance().blendState()->setBlendState(bd);
//
//    auto proj = mCamera->getView();
//    //proj.inverse();
//    proj = Matrix4::identity;
//    proj.m[0][0] = 2.f / Window::width();
//    proj.m[1][1] = -2.f / Window::height();
//    mSprite->draw(proj);
//}
