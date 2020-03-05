#pragma once

#include <string>

#define TITLE L"DirectX"

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}
#define MFAIL(code,string) if(FAILED(code) ) { MessageBox(0, string, L"error",MB_OK); return E_FAIL; }
#define MFALSE(code,string) if(!(code) ) { MessageBox(0, string, L"error",MB_OK); return E_FAIL; }
#define MSG(t) MessageBox(0, t, 0, MB_OK);
#define ALIGN16 _declspec(align(16))

#include "DirectXIncLib.h"
#include <memory>

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
