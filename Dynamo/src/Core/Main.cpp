#include "dynamopch.h"
#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    try {
        App app(L"Dynamo", 1280, 720);
        app.Run();
    }
    catch (const DynamoException& e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}