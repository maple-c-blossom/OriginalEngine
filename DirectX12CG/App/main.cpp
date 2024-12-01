#include "App.h"
//#include "Capture.h"
#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma endregion pragma comment


int32_t main()
{
    std::make_unique<MCB::App>()->Run();
	return 0;
}