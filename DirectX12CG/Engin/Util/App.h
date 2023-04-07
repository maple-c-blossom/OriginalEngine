#pragma once
#include "SceneManager.h"
#include "Scene.h"
#include "FPS.h"
namespace MCB
{
	class App
	{
	private:
		DxWindow* dxWindow = nullptr;
		FPS* fps = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
		Input* input = nullptr;
		std::unique_ptr<Depth> depth = nullptr;
		std::unique_ptr <RootParameter> rootparams = nullptr;
		std::unique_ptr <PipeLineManager> pipeline = nullptr;
		std::unique_ptr <SceneManager> scene = nullptr;
	public:
		void Init();
		void MainLoop();
		void Final();

		void Run();
	};

}