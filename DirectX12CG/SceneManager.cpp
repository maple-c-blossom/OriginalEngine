#include "SceneManager.h"
#include "DxWindow.h"
#include "Scene.h"
#include "Draw.h"

MCB::SceneManager::SceneManager(RootParameter* root, Depth* depth, PipeLineManager* pipeline)
{
	this->root = root;
	this->pipeline = pipeline;
	this->depth = depth;
	damyTexture = texmanager->CreateNoTextureFileIsTexture();
	loadBackGroundTex = texmanager->CreateNoTextureFileIsTexture();
	loadTex = texmanager->LoadTexture(L"Resources\\reimu.png");
	loadBackGround.InitMatProje();
	loadBackGround = loadBackGround.CreateSprite();
	loadBackGround.color = { 0,0,0,0 };
	loadSprite.InitMatProje();
	loadSprite = loadSprite.CreateSprite();
	InitRand();
	imgui.Init();
	scene = new Scene(this->root, this->depth, this->pipeline);
}

MCB::SceneManager::~SceneManager()
{
	delete scene;
	imgui.Final();
}

void MCB::SceneManager::Initialize()
{
	scene->Initialize();

}

void MCB::SceneManager::Update()
{
	if (isInitialized)//�Q�[����ʂւ̏���
	{
		if (isChengeSceneTimer <= 0)
		{
			scene->Update();
			loadBackGround.color.w = 0;
			loadSprite.color.w = loadBackGround.color.w;
			isChengeSceneTimer = 0;
		}
		if (scene->GetIsSceneEnd())
		{
			//���[�h��ʂւ̑J�ڃA�j���[�V�����I����
			if ( isChengeSceneTimer >= isChengeSceneTime)
			{
				isInitialized = false;
				sceneInitialize = std::async(std::launch::async, [this] {return sceneChenge(); });
			}
			else//���[�h��ʂւ̑J�ڃA�j���[�V��������
			{
				isChengeSceneTimer++;
				if (isChengeSceneTimer >= isChengeSceneTime)isChengeSceneTimer = isChengeSceneTime;
				loadBackGround.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
				loadSprite.color.w = loadBackGround.color.w;
				
				angle = 0;
			}
		}
		else//�Q�[����ʂւ̑J�ڃA�j���[�V��������
		{
			if (isChengeSceneTimer > 0)
			{
				scene->MatrixUpdate();
				isChengeSceneTimer--;
				if (isChengeSceneTimer <= 0)isChengeSceneTimer = 0;
				loadBackGround.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
				loadSprite.color.w = loadBackGround.color.w;
				angle++;
				if (angle >= 360)
				{
					angle = 0;
				}
				loadSprite.rotation = angle;
			}
		}
	}
	else//���[�h��ʂ̏���
	{
		loadBackGround.color.w = 1;
		loadSprite.color.w = loadBackGround.color.w;
		angle++;
		if (angle >= 360)
		{
			angle = 0;
		}
		loadSprite.rotation = angle;
	}
}

void MCB::SceneManager::Draw()
{
	Draw::GetInstance()->PreDraw(*scene->GetDepth(), *scene->Getpipeline()->Getpipeline(0,Alpha), scene->clearColor);
	if (isInitialized)//�Q�[�����
	{
		//3D�`��
		scene->Draw();
		//�p�[�e�B�N��
		scene->Getpipeline()->SetParticlePipeLine(Alpha);
		scene->ParticleDraw();
		//�X�v���C�g
		scene->Getpipeline()->SetSpritePipeLine(Alpha);
		scene->SpriteDraw();



		loadBackGround.SpriteDraw(*loadBackGroundTex->texture, DxWindow::GetInstance()->window_width / 2, DxWindow::GetInstance()->window_height / 2, DxWindow::GetInstance()->window_width, DxWindow::GetInstance()->window_height);
#ifdef _DEBUG
		scene->ImGuiUpdate();
		scene->ImGuiDraw();
#endif 
	}
	else//���[�h���
	{
		scene->Getpipeline()->SetSpritePipeLine(Alpha);
		loadBackGround.SpriteDraw(*loadBackGroundTex->texture, DxWindow::GetInstance()->window_width / 2, DxWindow::GetInstance()->window_height / 2, DxWindow::GetInstance()->window_width, DxWindow::GetInstance()->window_height);
		loadSprite.SpriteDraw(*loadTex->texture, DxWindow::GetInstance()->window_width / 2, DxWindow::GetInstance()->window_height / 2);
	}  
	Draw::GetInstance()->PostDraw();

}

void MCB::SceneManager::sceneChenge()
{
	IScene* nextScene = scene->GetNextScene();
	nextScene->Initialize();
	delete scene;
	scene = nextScene;
	texmanager->Erase();
	isInitialized = true;

}

MCB::IScene* MCB::SceneManager::GetScene()
{
	return scene;
}
