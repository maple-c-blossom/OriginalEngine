#pragma once
#include "IScene.h"
#include <future>
namespace MCB
{
	class SceneManager
	{
	private:

		IScene* scene;
		RootParameter* root;
		Depth* depth;
		TextureManager* texmanager = TextureManager::GetInstance();
		PipeLineManager* pipeline;
		std::future<void> sceneInitialize;
		static const int isChengeSceneTime = 30;
		int isChengeSceneTimer = 0;
		TextureCell* damyTexture;
		TextureCell* loadTex;
		TextureCell* loadBackGroundTex;
		Sprite loadSprite;
		Sprite loadBackGround;
		ImguiManager imgui;
		bool isInitialized = true;
		int angle = 0;
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

