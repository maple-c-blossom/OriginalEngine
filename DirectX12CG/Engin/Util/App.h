#pragma once
#include "SceneManager.h"
#include "Scene.h"
#include "FPS.h"
namespace MCB
{
	class App
	{
	private:
		DxWindow* dxWindow_ = nullptr;
		FPS* fps_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Debug1> debugController_;
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue_;
		Input* input_ = nullptr;
		std::unique_ptr<Depth> depth_ = nullptr;
		std::unique_ptr <RootParameter> rootparams_ = nullptr;
		std::unique_ptr <PipeLineManager> pipeline_ = nullptr;
		std::unique_ptr <SceneManager> scene_ = nullptr;
	public:
		void Init();
		void MainLoop();
		void Final();

		void Run();
	};

}