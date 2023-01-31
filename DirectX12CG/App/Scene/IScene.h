#pragma once

#pragma region 標準.h include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <cassert>
#include <memory>
#include <DirectXTex.h>
#include <array>

#pragma endregion 標準.h include

#pragma region 自作.h include

#include "Input.h"
#include "DxWindow.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "WorldMatrix.h"
#include "Depth.h"
#include "Object3d.h"
#include "ObjectMaterial.h"
#include "RootParameter.h"
#include "Vertex.h"
#include "MCBMatrix.h"
#include "Util.h"
#include "Particle.h"
#include "Quaternion.h"
#include "Texture.h"
#include "PIpelineRootSignature.h"
#include "Draw.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Sound.h"
#include "Collider.h"
#include "LightGroup.h"
#include "FBXLoader.h"
#include "PipeLineManager.h"
#include "ImguiManager.h"
#include "Particle.h"//後々ParticleManagerに変更。Particle単体でテストする用
#include "ICamera.h"
#include "TextureManager.h"
#include <MyImgui/imgui.h>
#pragma endregion 自作.h include
#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

namespace MCB
{
	class IScene
	{

	protected:
#pragma region DirectX基礎機能群
		DxWindow* dxWindow = DxWindow::GetInstance();
		//DirectXクラス生成
		Dx12* dx = Dx12::GetInstance();
		//inputクラス生成
		Input* input = Input::GetInstance();

		ShaderResource* descriptor = ShaderResource::GetInstance();

		Depth* depth;
		//ルートパラメータ
		RootParameter* rootparamsPtr;

		PipeLineManager* pipeline;

		//PipelineRootSignature* obj3dPipelinePtr;

		//PipelineRootSignature* spritePipelinePtr;

		//PipelineRootSignature* particlePipelinePtr;

		LightGroup* lights = LightGroup::GetInstance();

		//描画前処理と描画後処理
		MCB::Draw* draw = Draw::GetInstance();

		//FBXLoader* fbxLoader = FBXLoader::GetInstance();
	
		//サウンドマネージャー
		SoundManager soundManager;
		//クリアカラー
#pragma endregion DirectX基礎機能群
		bool sceneEnd = false;

		IScene* nextScene = nullptr;

		ICamera* viewCamera;

		TextureManager* loader = TextureManager::GetInstance();
	public:
		ImguiManager imgui;
		float clearColor[4] = { 0.0f,0.25f, 0.5f,0.0f }; // 青っぽい色
		virtual ~IScene() {};
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void SpriteDraw() = 0;
		virtual void ParticleDraw() = 0;
		virtual void LoadModel() = 0;
		virtual void LoadTexture() = 0;
		virtual void LoadSound() = 0;
		virtual void Object3DInit() = 0;
		virtual void SpriteInit() = 0;
		virtual void ParticleInit() = 0;
		virtual void MatrixUpdate() = 0;
		void ImGuiDraw() { imgui.Draw(); }
		virtual void ImGuiUpdate() = 0;
		virtual IScene* GetNextScene() = 0;
		virtual void CheckAllColision() = 0;
		bool GetIsSceneEnd() { return sceneEnd; }

		Depth* GetDepth() { return depth; }
		//ルートパラメータ
		RootParameter* GetRootparamsPtr() { return rootparamsPtr; }
		
		PipeLineManager* Getpipeline() { return pipeline; }
	};

}

