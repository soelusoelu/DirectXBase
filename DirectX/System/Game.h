﻿#pragma once

#include <string>
#include "DirectXIncLib.h"
#include <memory>

#define TITLE L"DirectX"

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}
#define ALIGN16 _declspec(align(16))

class Main;
class Renderer;
class Window;

class Game {
public:
    Game();
    ~Game();
    void run(HINSTANCE);
    HRESULT init();
    void mainLoop();
    void fixFPS60();

public:
    static const int WINDOW_WIDTH = 1080;
    static const int WINDOW_HEIGHT = 720;

private:
    std::unique_ptr<Window> mWindow;
    std::shared_ptr<Renderer> mRenderer;
    std::unique_ptr<Main> mMain;
    HINSTANCE mInstance;
    HWND mhWnd;
};

void initDirectory(WCHAR* root);
void setRootDirectory();
void setAssetsDirectory();
void setShaderDirectory();
void setTextureDirectory();
void setSoundDirectory();
void setDataDirectory();
void setModelDirectory(const std::string& directry);
