#include "SceneManager.h"
#include "DxWindow.h"
#include "Scene.h"
#include "Draw.h"
using namespace std;

MCB::SceneManager::SceneManager(RootParameter* root, Depth* depth, PipeLineManager* pipeline)
{
	root_ = root;
	pipeline_ = pipeline;
	depth_ = depth;
	damyTexture_ = texmanager_->CreateNoTextureFileIsTexture();
	loadBackGroundTex_ = texmanager_->CreateNoTextureFileIsTexture();
	loadTex_ = texmanager_->LoadTexture(L"Resources\\reimu.png");
	loadBackGround_.InitMatProje();
	loadBackGround_.color_ = { 0,0,0,0 };
	loadSprite_.InitMatProje();
	InitRand();
	imgui_.Init();
	scene_ = make_unique<Scene>(root_, depth_, pipeline_);
}

MCB::SceneManager::~SceneManager()
{
	imgui_.Final();
	
}

void MCB::SceneManager::Initialize()
{
	scene_->Initialize();

}

void MCB::SceneManager::Update()
{
	if (isSceneReady_)//�Q�[����ʂւ̏���
	{
		if (isChengeSceneTimer_ <= 0)
		{
			scene_->Update();
			loadBackGround_.color_.w_= 0;
			loadSprite_.color_.w_ = loadBackGround_.color_.w_;
			isChengeSceneTimer_ = 0;
		}
		if (scene_->GetIsSceneEnd())
		{
			//���[�h��ʂւ̑J�ڃA�j���[�V�����I����
			if ( isChengeSceneTimer_ >= isChengeSceneTime_)
			{
				isSceneReady_ = false;
				OutputDebugStringW(L"asyncCall\n");
				sceneInitialize_ = std::async(std::launch::async, [this] {return sceneChenge(); });
				OutputDebugStringW(L"asyncCallEnd\n");
			}
			else//���[�h��ʂւ̑J�ڃA�j���[�V��������
			{
				isChengeSceneTimer_++;
				if (isChengeSceneTimer_ >= isChengeSceneTime_)isChengeSceneTimer_ = isChengeSceneTime_;
				loadBackGround_.color_.w_ = (float)Lerp(0.f, 1.f, (float)isChengeSceneTime_, (float)isChengeSceneTimer_);
				loadSprite_.color_.w_ = loadBackGround_.color_.w_;
				
				angle_ = 0;
			}
		}
		else//�Q�[����ʂւ̑J�ڃA�j���[�V��������
		{
			if (isChengeSceneTimer_ > 0)
			{
				scene_->MatrixUpdate();
				isChengeSceneTimer_--;
				if (isChengeSceneTimer_ <= 0)isChengeSceneTimer_ = 0;
				loadBackGround_.color_.w_ = (float)Lerp(0.f, 1.f, (float)isChengeSceneTime_, (float)isChengeSceneTimer_);
				loadSprite_.color_.w_ = loadBackGround_.color_.w_;
				angle_++;
				if (angle_ >= 360)
				{
					angle_ = 0;
				}
				loadSprite_.rotation_ = (float)angle_;
			}
		}
	}
	else//���[�h��ʂ̏���
	{
		loadBackGround_.color_.w_ = 1;
		loadSprite_.color_.w_ = loadBackGround_.color_.w_;
		angle_++;
		if (angle_ >= 360)
		{
			angle_ = 0;
		}
		loadSprite_.rotation_ = (float)angle_;
	}
}

void MCB::SceneManager::Draw()
{

		Draw::GetInstance()->PreDraw(scene_->GetDepth(), *scene_->Getpipeline().Getpipeline(0, Alpha), scene_->clearColor_);

	if (isSceneReady_)//�Q�[�����
	{
		scene_->PostEffectDraw();
		Draw::GetInstance()->PreDraw(scene_->GetDepth(), *scene_->Getpipeline().Getpipeline(0, Alpha), scene_->clearColor_);
		//3D�`��
		scene_->Draw();
		//�p�[�e�B�N��
		scene_->Getpipeline().SetParticlePipeLine(Alpha);
		scene_->ParticleDraw();
		//�X�v���C�g
	
		scene_->Getpipeline().SetSpritePipeLine(Alpha);
		scene_->SpriteDraw();



		loadBackGround_.SpriteDraw(*loadBackGroundTex_->texture,(float) DxWindow::GetInstance()->sWINDOW_WIDTH_ / 2,
		(float)DxWindow::GetInstance()->sWINDOW_HEIGHT_ / 2, (float)DxWindow::GetInstance()->sWINDOW_WIDTH_,
		(float)DxWindow::GetInstance()->sWINDOW_HEIGHT_);
//#ifdef _DEBUG
		scene_->ImGuiUpdate();
		scene_->ImGuiDraw();
//#endif 
	}
	else//���[�h���
	{
		scene_->Getpipeline().SetSpritePipeLine(Alpha);
		loadBackGround_.SpriteDraw(*loadBackGroundTex_->texture, (float)DxWindow::GetInstance()->sWINDOW_WIDTH_ / 2, 
			(float)DxWindow::GetInstance()->sWINDOW_HEIGHT_ / 2, (float)DxWindow::GetInstance()->sWINDOW_WIDTH_,
			(float)DxWindow::GetInstance()->sWINDOW_HEIGHT_);
		loadSprite_.SpriteDraw(*loadTex_->texture, (float)DxWindow::GetInstance()->sWINDOW_WIDTH_ / 2,
			(float)DxWindow::GetInstance()->sWINDOW_HEIGHT_ / 2);
	}  
	Draw::GetInstance()->PostDraw();
}

void MCB::SceneManager::sceneChenge()
{
	OutputDebugStringW(L"SceneChengeIn\n");
	unique_ptr<IScene> nextScene = scene_->GetNextScene();
	OutputDebugStringW(L"SceneChengeGetNext\n");
	nextScene->Initialize();
	OutputDebugStringW(L"SceneChengeInitComp\n");
	scene_ = std::move(nextScene);
	OutputDebugStringW(L"SceneChengeComp\n");
	texmanager_->Erase();
	OutputDebugStringW(L"SceneChengeTexErase\n");
	isSceneReady_ = true;
	OutputDebugStringW(L"SceneChengeEnd\n");
}

MCB::IScene* MCB::SceneManager::GetScene()
{
	return scene_.get();
}
