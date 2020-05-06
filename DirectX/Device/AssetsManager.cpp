#include "AssetsManager.h"
#include "Sound.h"
#include "../Mesh/FBX.h"
#include "../Mesh/OBJ.h"
#include "../Shader/Shader.h"
#include "../System/Texture.h"
#include "../Utility/FileUtil.h"

AssetsManager::AssetsManager() :
    mSoundBase(std::make_unique<SoundBase>()) {
}

AssetsManager::~AssetsManager() = default;

std::shared_ptr<Shader> AssetsManager::createShader(const std::string & fileName) {
    std::shared_ptr<Shader> shader = nullptr;
    auto itr = mShaders.find(fileName);
    if (itr != mShaders.end()) { //既に読み込まれている
        shader = itr->second;
    } else { //初読み込み
        shader = std::make_shared<Shader>(fileName);
        mShaders.emplace(fileName, shader);
    }
    return shader;
}

std::shared_ptr<Texture> AssetsManager::createTexture(const std::string & fileName, bool isSprite) {
    std::shared_ptr<Texture> texture = nullptr;
    auto itr = mTextures.find(fileName);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        texture = std::make_shared<Texture>(fileName, isSprite);
        mTextures.emplace(fileName, texture);
    }
    return texture;
}

std::shared_ptr<Sound> AssetsManager::createBGM(const std::string & fileName) {
    std::shared_ptr<Sound> sound = nullptr;
    auto itr = mSounds.find(fileName);
    if (itr != mSounds.end()) { //既に読み込まれている
        sound = itr->second;
    } else { //初読み込み
        sound = std::make_shared<Sound>();
        mSoundBase->load(fileName, &sound);
        mSounds.emplace(fileName, sound);
    }
    return sound;
}

std::shared_ptr<Sound> AssetsManager::createSE(const std::string & fileName) {
    auto sound = createBGM(fileName);
    mSoundBase->createSourceVoice(&sound);
    return sound;
}

std::shared_ptr<IMeshLoader> AssetsManager::createMesh(const std::string & fileName) {
    std::shared_ptr<IMeshLoader> mesh = nullptr;
    auto itr = mMeshLoaders.find(fileName);
    if (itr != mMeshLoaders.end()) { //既に読み込まれている
        mesh = itr->second;
    } else { //初読み込み
        auto ext = FileUtil::getFileExtension(fileName);
        if (ext == ".obj") {
            mesh = std::make_shared<OBJ>();
        } else if (ext == ".fbx") {
            mesh = std::make_shared<FBX>();
        }
        mesh->perse(fileName);
        mMeshLoaders.emplace(fileName, mesh);
    }
    return mesh;
}
