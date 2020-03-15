#pragma once

#include "DirectXIncLib.h"
#include <rapidjson/document.h>
#include <memory>

#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = nullptr; }
#define SAFE_DELETE(x) if(x) { delete (x); (x) = nullptr; }
#define SAFE_DELETE_ARRAY(x) if(x) { delete[] (x); (x) = nullptr; }
#define ALIGN16 _declspec(align(16))

class FPSCounter;
class Renderer;
class SceneManager;
class Window;

class Game {
public:
    Game();
    ~Game();
    void run(HINSTANCE);
    bool initialize();
    void mainLoop();
    static void quit();
    void loadProperties(const rapidjson::Value& inObj);

private:
    std::unique_ptr<Window> mWindow;
    std::shared_ptr<Renderer> mRenderer;
    std::unique_ptr<SceneManager> mSceneManager;
    std::unique_ptr<FPSCounter> mFPSCounter;
    HINSTANCE mInstance;
    HWND mhWnd;
};
