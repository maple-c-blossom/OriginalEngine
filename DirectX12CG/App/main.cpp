#include "App.h"
#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma endregion pragma comment


int32_t WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,_In_ int32_t nCmdShow)
{
	static_cast< void >( hInstance );
	static_cast< void >( hPrevInstance );
	static_cast< void >( lpCmdLine );
	static_cast< void >( nCmdShow );
    std::make_unique<MCB::App>()->Run();
	return 0;
}