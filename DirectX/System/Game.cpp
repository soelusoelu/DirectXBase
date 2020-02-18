#include "Game.h"
#include "Texture.h"
#include "Window.h"
#include "../Main.h"
#include "../Device/DrawString.h"
#include "../Device/Random.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Utility/Input.h"

WCHAR szRootPath[1024] = { 0 };

Game::Game() {
    ZeroMemory(this, sizeof(Game));
}

Game::~Game() {
    DrawString::end();
    Input::end();
    Texture::end();
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

    mRenderer = std::make_shared<Renderer>(mhWnd);
    mRenderer->initialize();

    Random::init();
    Input::init(mhWnd);
    DrawString::initialize(mRenderer);

    mMain = std::make_unique<Main>(mRenderer);

    return S_OK;
}

void Game::mainLoop() {
    mRenderer->clearRenderTarget();
    mRenderer->clearDepthStencilView();

    Input::update();

    mMain->update();
    mMain->draw();

    fixFPS60();
    mRenderer->present();
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

void setOBJDirectory() {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));
    wcscat_s(tmp, L"\\Assets/OBJ");
    SetCurrentDirectory(tmp);
}
