#include "Game.h"
#include "DirectX.h"
#include "Texture.h"
#include "Window.h"
#include "../Device/DrawString.h"
#include "../Device/FPSCounter.h"
#include "../Device/Random.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../Scene/SceneManager.h"
#include "../Utility/Debug.h"
#include "../Utility/FileUtil.h"
#include "../Utility/Input.h"
#include "../Utility/StringUtil.h"

WCHAR szRootPath[1024] = { 0 };

Game::Game() {
    ZeroMemory(this, sizeof(Game));
}

Game::~Game() {
    Input::end();
    Texture::end();
    Debug::finalize();
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
    mWindow->initWindow(mInstance, 0, 0, WINDOW_DEBUG_WIDTH, WINDOW_DEBUG_HEIGHT, TITLE);
    mhWnd = mWindow->gethWnd();

    Singleton<DirectX>::instance().initialize(mhWnd);
    mRenderer = std::make_shared<Renderer>();
    mRenderer->initialize();

    Random::init();
    Input::initialize(mhWnd);
    Debug::initialize(mRenderer);

    mFPSCounter = std::make_unique<FPSCounter>(mRenderer, 60.f);
    mSceneManager = std::make_unique<SceneManager>(mRenderer);

    return S_OK;
}

void Game::mainLoop() {
    Singleton<DirectX>::instance().clearRenderTarget();
    Singleton<DirectX>::instance().clearDepthStencilView();

    Input::update();

    mSceneManager->update();
    mSceneManager->draw();

    mFPSCounter->fixedFrame();
    Singleton<DirectX>::instance().present();
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

void setModelDirectory(const std::string& directry) {
    WCHAR tmp[1024] = { 0 };
    wcsncpy_s(tmp, szRootPath, wcslen(szRootPath));

    const std::string modelDir = "\\Assets/Model/";
    auto modelPath = modelDir + FileUtil::getDirectryFromFilePath(directry);

    auto dir = StringUtil::charToWchar(modelPath.c_str());

    wcscat_s(tmp, dir);
    SetCurrentDirectory(tmp);
}
