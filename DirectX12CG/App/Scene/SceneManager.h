#pragma once
#include "IScene.h"
#include <future>
namespace MCB
{
	class SceneManager
	{
	private:

		std::shared_ptr<IScene> scene;
		RootParameter* root;
		Depth* depth;
		TextureManager* texmanager = TextureManager::GetInstance();
		PipeLineManager* pipeline;
		std::future<void> sceneInitialize;
		static const int32_t isChengeSceneTime = 30;
		int32_t isChengeSceneTimer = 0;
		TextureCell* damyTexture;
		TextureCell* loadTex;
		TextureCell* loadBackGroundTex;
		Sprite loadSprite;
		Sprite loadBackGround;
		PostEffect postEffect;
		ImguiManager imgui;
		bool isInitialized = true;
		int32_t angle = 0;
	public:
		SceneManager(RootParameter* root, Depth* depth, PipeLineManager* pipeline);
		~SceneManager();
		void Initialize();
		void Update();
		void Draw();
		void sceneChenge();
		IScene* GetScene();
	};

}

