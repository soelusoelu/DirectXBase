#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

class Camera;
class MeshLoader;
class Renderer;
class Shader;
class Sound;
class SoundBase;
class Texture;

class AssetsManager {
public:
    AssetsManager();
    ~AssetsManager();
    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<Texture> createTexture(const std::string& fileName, bool isSprite = true);
    std::shared_ptr<Sound> createBGM(const std::string& fileName);
    std::shared_ptr<Sound> createSE(const std::string& fileName);
    std::shared_ptr<MeshLoader> createMesh(std::shared_ptr<Renderer> renderer, const std::string& fileName);

private:
    std::unique_ptr<SoundBase> mSoundBase;

    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Sound>> mSounds;
    std::unordered_map<std::string, std::shared_ptr<MeshLoader>> mMeshLoaders;
};
