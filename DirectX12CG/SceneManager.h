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
		PipelineRootSignature* pipeline;//Obj3D
		PipelineRootSignature* pipeline1;//Sprite
		std::future<void> sceneInitialize;
		static const int isChengeSceneTime = 120;
		int isChengeSceneTimer = 0;
		Texture loadTex;
		Texture loadBackGroundTex;
		Sprite loadSprite;
		Sprite loadBackGround;
		bool isInitialized = true;
		int angle = 0;
	public:
		SceneManager(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		~SceneManager();
		void Initialize();
		void Update();
		void Draw();
		void sceneChenge();
		IScene* GetScene();
	};

}

