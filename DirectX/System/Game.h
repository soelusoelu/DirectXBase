#pragma once

#include "SystemInclude.h"
#include <rapidjson/document.h>
#include <memory>

class FPSCounter;
class Renderer;
class SceneManager;
class Window;

class Game {
public:
    Game();
    ~Game();
    //アプリケーションの実行
    void run(HINSTANCE);
    //グローバルファイルを読み込む
    void loadProperties(const rapidjson::Value& inObj);

    //アプリケーションの終了
    static void quit();

private:
    //各クラスの初期化
    bool initialize();
    //メインループ
    void mainLoop();

private:
    std::unique_ptr<Window> mWindow;
    std::shared_ptr<Renderer> mRenderer;
    std::unique_ptr<SceneManager> mSceneManager;
    std::unique_ptr<FPSCounter> mFPSCounter;
    HINSTANCE mInstance;
    HWND mhWnd;
};
