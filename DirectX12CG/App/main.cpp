#include "App.h"
int32_t WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,_In_ int32_t nCmdShow)
{
    std::make_unique<MCB::App>()->Run();
	return 0;
}