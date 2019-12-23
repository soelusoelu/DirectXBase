#include "Game.h"
#include <windows.h>
//���������[�N���o�p
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG
//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT) {
    Game* game = new Game();
    if (!game) {
        MessageBox(0, L"�N���X�������s �A�v�����I�����܂�", NULL, MB_OK);
        return 0;
    }

    game->run(hInstance);

    delete game;

    //���������[�N���o�֐�
#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG

    return 0;
}