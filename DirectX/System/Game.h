﻿#pragma once

#include <string>
#include "DirectXIncLib.h"
#include <memory>

#define TITLE L"DirectX"

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}
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

public:
    static const int WINDOW_WIDTH = 1080;
    static const int WINDOW_HEIGHT = 720;
    static const int WINDOW_DEBUG_WIDTH = 1500;
    static const int WINDOW_DEBUG_HEIGHT = 1000;

private:
    std::unique_ptr<Window> mWindow;
    std::shared_ptr<Renderer> mRenderer;
    std::unique_ptr<SceneManager> mSceneManager;
    std::unique_ptr<FPSCounter> mFPSCounter;
    HINSTANCE mInstance;
    HWND mhWnd;
};
