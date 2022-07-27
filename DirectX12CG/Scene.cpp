#include "Scene.h"

using namespace MCB;
using namespace DirectX;

MCB::Scene::~Scene()
{
    soundManager.ReleasexAudio2();
    soundManager.AllDeleteSound();
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;
}

#pragma region 通常変数の初期化と3Dオブジェクトの初期化
void MCB::Scene::Initialize()
{
    matView.CreateMatrixView(XMFLOAT3(0.0f, 40.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    InitRand();
    soundManager.PlaySoundWave(testSound, loopFlag);



}

void MCB::Scene::Object3DInit()
{
    human;
    human.Init(BoxModel);

    ground;
    ground.Init();
    ground.model = groundModel;
    ground.scale = { 4,4,4 };
    ground.position = { 0,-15,0 };
    ;
    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };

    box.Init();
    box.model = BoxModel;
    box.scale = {3,3,3};
    box.position.x = 30;

    ray.Init();
    ray.model = BoxModel;
    ray.scale = { 1,1,30 };
    ray.SetCollider(62, 1, { 0,0,1 });

    sphere.Init();
    sphere.model = BoxModel;
    sphere.SetCollider(1);
    sphere.position.x = 20;
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{
	BoxModel = new Model("Box");

	groundModel = new Model("ground");

	skydomeModel = new Model("skydome");
}

void MCB::Scene::LoadTexture()
{
	testTex.CreateTexture(L"Resources\\reimu.png");
	debugTextTexture.CreateTexture(L"Resources\\debugfont.png");
    zoomTex.CreateTexture(L"Resources\\reticle.png");
    scopeTex.CreateTexture(L"Resources\\scope.png");

}

void MCB::Scene::LoadSound()
{
	testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::Scene::SpriteInit()
{
    sprite.InitMatProje();
    sprite = sprite.CreateSprite();
    zoomSprite.InitMatProje();
    zoomSprite = zoomSprite.CreateSprite();
    scopeSprite.InitMatProje();
    scopeSprite = scopeSprite.CreateSprite();
    debugText.Init(&debugTextTexture);

}
#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
    if (input->IsKeyDown(DIK_D))
    {
        sphere.position.x += 0.1f;
    }

    if (input->IsKeyDown(DIK_A))
    {
        sphere.position.x -= 0.1f;
    }

    if (input->IsKeyDown(DIK_W))
    {
        sphere.position.z += 0.1f;
    }

    if (input->IsKeyDown(DIK_S))
    {
        sphere.position.z -= 0.1f;
    }
    
    if (input->IsKeyDown(DIK_SPACE))
    {
        sphere.position.y += 0.1f;
    }
    
    if (input->IsKeyDown(DIK_LCONTROL))
    {
        sphere.position.y -= 0.1f;
    }



    if (input->IsKeyDown(DIK_UP))
    {
        volume ++;
    }

    if (input->IsKeyDown(DIK_DOWN))
    {
        volume--;
    }
    if (input->IsKeyDown(DIK_LEFT))
    {
        matView.eye.x += 0.1f;
    }

    if (input->IsKeyDown(DIK_RIGHT))
    {
        matView.eye.x -= 0.1f;
    }

    ray.ColliderUpdate();
    sphere.ColliderUpdate();

    soundManager.SetVolume(volume, testSound);

    if (input->IsKeyTrigger(DIK_T))
    {
        soundManager.StopSoundWave(testSound);
    }

    if (input->IsKeyTrigger(DIK_Y))
    {
        //soundManager.StopSoundWave(testSound);
        soundManager.PlaySoundWave(testSound, loopFlag);
    }

    if (input->IsKeyTrigger(DIK_U))
    {
        loopFlag = !loopFlag;
        soundManager.StopSoundWave(testSound);
    }

    if (input->IsKeyTrigger(DIK_F))
    {
        soundManager.StopSoundWave(test2Sound);
        soundManager.PlaySoundWave(test2Sound, false);
    }
    //行列変換
    MatrixUpdate();
}

void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    //3Dオブジェクト
    Skydorm.Draw();
    //human.Draw();
    ray.Draw();
    sphere.Draw();
    box.Draw();
    //スプライト
    sprite.SpriteCommonBeginDraw(*spritePipelinePtr);
    if (CalcRaySphere(ray.collider, sphere))
    {
        debugText.Print(0, 100, 5, "Hit");
    }
    debugText.Print(0, 20, 1, "Move:WASD,yMove:Space,LCONTROL cameraMove:ArrowKey");
    debugText.Print(0, 60, 1, "Sound: stop->T, start->Y");
    debugText.Print(0, 100, 1, "SE:start->F,LoopChengeandStop->U,LootFlag:%s", (loopFlag ? "true" : "false"));
    debugText.AllDraw();
    draw.PostDraw();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView();
    human.UpDate(matView, matProjection);
    Skydorm.Updata(matView, matProjection);
    ground.Updata(matView, matProjection);
    box.Updata(matView, matProjection,true);
    ray.Updata(matView, matProjection);
    sphere.Updata(matView, matProjection);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    depth = depthptr;
}

