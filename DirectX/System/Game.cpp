#include "Game.h"
#include "Direct3D11.h"
#include "Window.h"
#include "../Main.h"
#include "../Device/Random.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Utility/Input.h"
#include <iostream>

WCHAR szRootPath[1024] = { 0 };

Game::Game() {
    ZeroMemory(this, sizeof(Game));
}

Game::~Game() {
    Input::end();
}

void Game::run(HINSTANCE hInstance) {
    WCHAR dir[1024];
    GetCurrentDirectory(sizeof(dir), dir);
    initDirectory(dir);

    mInstance = hInstance;
    if (FAILED(init())) {
        return;
    }
    ShowWindow(mhWnd, SW_SHOW);
    UpdateWindow(mhWnd);
    // メッセージループ
    MSG msg = { 0 };
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            mainLoop();
        }
    }
}

HRESULT Game::init() {
    mWindow = std::make_unique<Window>();
    if (!mWindow) {
        return E_FAIL;
    }
    MFAIL(mWindow->initWindow(mInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, TITLE), L"ウィンドウ作成失敗");
    mhWnd = mWindow->gethWnd();

    mD3D11 = std::make_unique<Direct3D11>();
    if (!mD3D11) {
        MSG(L"Direct3Dの初期化失敗");
        return E_FAIL;
    }
    MFALSE(mD3D11->init(mhWnd), L"Direct3D初期化失敗");

    mRenderer = std::make_shared<Renderer>(mD3D11->device(), mD3D11->deviceContext());
    mMain = std::make_unique<Main>(mRenderer);

    Random::init();
    Input::init(mhWnd);

    return S_OK;
}

void Game::mainLoop() {
    mD3D11->clear();

    Input::update();

    mMain->update();
    mMain->draw();

    fixFPS60();
    mD3D11->present();
}

void Game::fixFPS60() {
    static LARGE_INTEGER frq = { 0 }, previousTime = { 0 }, currentTime = { 0 };
    double time = 0;

    while (time < 16.6666) //1000ms / 60frame = 16.6666 
    {
        QueryPerformanceFrequency(&frq);

        QueryPerformanceCounter(&currentTime);
        time = currentTime.QuadPart - previousTime.QuadPart;
        time *= 1000.0 / static_cast<double>(frq.QuadPart);
    }
    float deltaTime = static_cast<float>(time / 1000.0);
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    Time::deltaTime = deltaTime;
    previousTime = currentTime;
}



void initDirectory(WCHAR* root) {
    wcsncpy_s(szRootPath, root, wcslen(root));
}

void setRootDirectory() {
    SetCurrentDirectory(szRootPath);
}

void setAssetsDirectory() {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));
    wcscat_s(tmp, L"\\Assets");
    SetCurrentDirectory(tmp);
}

void setShaderDirectory() {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));
    wcscat_s(tmp, L"\\Shader");
    SetCurrentDirectory(tmp);
}

void setTextureDirectory() {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));
    wcscat_s(tmp, L"\\Assets/Texture");
    SetCurrentDirectory(tmp);
}

void setSoundDirectory() {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));
    wcscat_s(tmp, L"\\Assets/Sound");
    SetCurrentDirectory(tmp);
}

void setDataDirectory() {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));
    wcscat_s(tmp, L"\\Assets/Data");
    SetCurrentDirectory(tmp);
}
