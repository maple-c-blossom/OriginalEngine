#include "SceneManager.h"
#include "DxWindow.h"
#include "Scene.h"
#include "Draw.h"

MCB::SceneManager::SceneManager(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	this->root = root;
	this->pipeline = pipeline;
	this->pipeline1 = pipeline1;
	this->depth = depth;
	loadTex.CreateTexture(L"Resources\\reimu.png");
	loadBackGroundTex.CreateNoTextureFileIsTexture();
	loadBackGround.InitMatProje();
	loadBackGround = loadBackGround.CreateSprite();
	loadBackGround.color = { 0,0,0,0 };
	loadSprite.InitMatProje();
	loadSprite = loadSprite.CreateSprite();
	InitRand();
	scene = new Scene(this->root, this->depth, this->pipeline, this->pipeline1);
}

MCB::SceneManager::~SceneManager()
{
	delete scene;
}

void MCB::SceneManager::Initialize()
{
	scene->Initialize();
}

void MCB::SceneManager::Update()
{
	if (isInitialized)//ゲーム画面への処理
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
			//ロード画面への遷移アニメーション終了後
			if ( isChengeSceneTimer >= isChengeSceneTime)
			{
				isInitialized = false;
				sceneInitialize = std::async(std::launch::async, [this] {return sceneChenge(); });
			}
			else//ロード画面への遷移アニメーション処理
			{
				isChengeSceneTimer++;
				if (isChengeSceneTimer >= isChengeSceneTime)isChengeSceneTimer = isChengeSceneTime;
				loadBackGround.color.w = Lerp(0, 1, isChengeSceneTime, isChengeSceneTimer);
				loadSprite.color.w = loadBackGround.color.w;
				
				angle = 0;
			}
		}
		else//ゲーム画面への遷移アニメーション処理
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
	else//ロード画面の処理
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
	Draw::GetInstance()->PreDraw(*scene->GetDepth(), *scene->GetObj3dPipelinePtr(), scene->clearColor);
	if (isInitialized)//ゲーム画面
	{
		//3D描画
		scene->Draw();
		//スプライト
		loadBackGround.SpriteCommonBeginDraw(*scene->GetSpritePipelinePtr());
		scene->SpriteDraw();
		loadBackGround.SpriteDraw(loadBackGround, loadBackGroundTex, DxWindow::GetInstance()->window_width / 2, DxWindow::GetInstance()->window_height / 2, DxWindow::GetInstance()->window_width, DxWindow::GetInstance()->window_height);
	}
	else//ロード画面
	{
		loadBackGround.SpriteCommonBeginDraw(*scene->GetSpritePipelinePtr());
		loadBackGround.SpriteDraw(loadBackGround, loadBackGroundTex, DxWindow::GetInstance()->window_width / 2, DxWindow::GetInstance()->window_height / 2, DxWindow::GetInstance()->window_width, DxWindow::GetInstance()->window_height);
		loadSprite.SpriteDraw(loadSprite, loadTex, DxWindow::GetInstance()->window_width / 2, DxWindow::GetInstance()->window_height / 2);
	}  
	Draw::GetInstance()->PostDraw();

}

void MCB::SceneManager::sceneChenge()
{
	IScene* nextScene = scene->GetNextScene();
	nextScene->Initialize();
	delete scene;
	scene = nextScene;
	isInitialized = true;

}

MCB::IScene* MCB::SceneManager::GetScene()
{
	return scene;
}
