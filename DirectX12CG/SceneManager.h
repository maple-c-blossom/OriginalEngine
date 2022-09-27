#pragma once
#include "IScene.h"

namespace MCB
{
	class SceneManager
	{
	private:
		IScene* scene;
	public:
		SceneManager();
		~SceneManager();
		void Initialize();
		void Update();
		void Draw();
	};

}

