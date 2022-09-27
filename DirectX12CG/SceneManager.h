#pragma once
#include "IScene.h"

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
	public:
		SceneManager(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		~SceneManager();
		void Initialize();
		void Update();
		void Draw();
	};

}

