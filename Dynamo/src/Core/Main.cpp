#include "dynamopch.h"
#include "App.h"
#include "GUI/Gui.h"

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    Gui::Init();

    try {
        App app("Dynamo", 1280, 720);
        return app.Run();
    }
    catch (const DynamoException& e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    
    Gui::Shutdown();
}