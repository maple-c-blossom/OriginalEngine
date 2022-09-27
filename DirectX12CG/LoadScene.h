#pragma once
#include "IScene.h"

namespace MCB
{
	class LoadScene :public IScene
	{

	public:
		IScene* nextScene = nullptr;

		void Initialize() override;

		//�e�������n�֐��Q--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		
		//---------------
		void MatrixUpdate();
		void Update() override;
		void Draw() override;
	};

}

