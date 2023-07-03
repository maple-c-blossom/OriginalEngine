#pragma once
#include "IScene.h"
#include <future>
namespace MCB
{
	class SceneManager
	{
	private:

		std::unique_ptr<IScene> scene_;
		RootParameter* root_;
		Depth* depth_;
		TextureManager* texmanager_ = TextureManager::GetInstance();
		PipeLineManager* pipeline_;
		std::future<void> sceneInitialize_;
		static const int32_t isChengeSceneTime_ = 30;
		int32_t isChengeSceneTimer_ = 0;
		TextureCell* damyTexture_;
		TextureCell* loadTex_;
		TextureCell* loadBackGroundTex_;
		Sprite loadSprite_;
		Sprite loadBackGround_;
		PostEffect postEffect_;
		ImguiManager imgui_;
		bool isSceneReady_ = true;
		int32_t angle_ = 0;
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

