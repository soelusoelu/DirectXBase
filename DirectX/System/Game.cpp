﻿#include "Game.h"
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
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include "../Utility/Input.h"
#include "../Utility/LevelLoader.h"

Game::Game() {
    ZeroMemory(this, sizeof(Game));
}

Game::~Game() {
    Input::end();
    Texture::end();
    Debug::finalize();
}

void Game::run(HINSTANCE hInstance) {
    Singleton<Directory>::instance().initialize();

    mInstance = hInstance;
    if (!initialize()) {
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

bool Game::initialize() {
    mWindow = std::make_unique<Window>();
    mRenderer = std::make_shared<Renderer>();
    mFPSCounter = std::make_unique<FPSCounter>(mRenderer);
    Debug::initialize(mRenderer);
    Singleton<LevelLoader>::instance().loadGlobal(this, "Global.json");

    mWindow->initWindow(mInstance, 0, 0);
    mhWnd = mWindow->gethWnd();

    Singleton<DirectX>::instance().initialize(mhWnd);
    mRenderer->initialize();

    Random::initialize();
    Input::initialize(mhWnd);

    mSceneManager = std::make_unique<SceneManager>(mRenderer);

    return true;
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

void Game::quit() {
    PostQuitMessage(0);
}

void Game::loadProperties(const rapidjson::Value& inObj) {
    mWindow->loadProperties(inObj);
    mRenderer->loadProperties(inObj);
    mFPSCounter->loadProperties(inObj);
    Debug::loadProperties(inObj);
}
