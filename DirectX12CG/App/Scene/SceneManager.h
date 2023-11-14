#pragma once
#include "IScene.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <future>
WarningIgnoreEnd
namespace MCB
{
	class SceneManager
	{
		class AnimationModel;
		class Object3d;
	private:
		std::unique_ptr<IScene> scene_;
		RootParameter* root_;
		Depth* depth_;
		TextureManager* texmanager_ = TextureManager::GetInstance();
		PipeLineManager* pipeline_;
		std::future<void> sceneInitialize_;
		static const int32_t isChengeSceneTime_ = 30;
		int32_t isChengeSceneTimer_ = 0;
		Byte4 pad;
		TextureCell* damyTexture_;
		TextureCell* loadTex_;
		TextureCell* loadBackGroundTex_;
		int64_t pad2;
		Sprite loadSprite_;
		Sprite loadBackGround_;
		PostEffect postEffect_;
		ImguiManager imgui_;
		bool isSceneReady_ = true;
		Byte2 pad3;
		int32_t angle_ = 0;
		int64_t pad4;
	public:
		SceneManager(RootParameter* root, Depth* depth, PipeLineManager* pipeline);
		SceneManager(const SceneManager&)=delete;
		SceneManager& operator=(const SceneManager&) = delete;
		~SceneManager();
		void Initialize();
		void Update();
		void Draw();
		void sceneChenge();
		IScene* GetScene();
	};

}

