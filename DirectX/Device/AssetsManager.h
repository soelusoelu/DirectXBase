#pragma once

#include "../Mesh/IMeshLoader.h"
#include "../Utility/Singleton.h"
#include <memory>
#include <string>
#include <unordered_map>

class Shader;
class Sound;
class SoundBase;
class Texture;

class AssetsManager {
    friend class Singleton<AssetsManager>;

private:
    AssetsManager();
    ~AssetsManager();

public:
    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<Texture> createTexture(const std::string& fileName, bool isSprite = true);
    std::shared_ptr<Sound> createSound(const std::string& fileName);
    std::shared_ptr<IMeshLoader> createMesh(const std::string& fileName);

private:
    std::unique_ptr<SoundBase> mSoundBase;

    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Sound>> mSounds;
    std::unordered_map<std::string, std::shared_ptr<IMeshLoader>> mMeshLoaders;
};
