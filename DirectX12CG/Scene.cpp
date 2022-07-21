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
    matView.CreateMatrixView(XMFLOAT3(0.0f, 50.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    InitRand();
    //soundManager.PlaySoundWave(testSound, true);

    angle = 0;
    anglemove = 0.1f;

    isJump = false;
    jumpSpeed = 5;

    startQ.SetRota({ 0,1,0 }, 0);
    mainQ = startQ;
    endQ.SetRota({ 1,1,1 }, 0.5);
    time = 0;
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
    box.scale = {4,4,4};

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
    if (input->IsKeyTrigger(DIK_SPACE))
    {
        time = 0;
        mainQ = startQ;
        endQ.SetRota({ (float)GetRand(0,10000) / 10000,(float)GetRand(0,10000) / 10000, 
            (float)GetRand(0,10000) / 10000 }, (float)GetRand(0, 10000) / 10000);
    }
    if (time < maxTime)
    {
        time++;
    }
    mainQ = mainQ.Slerp(startQ, endQ, time, maxTime);
    //行列変換
    MatrixUpdate();
}

void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    //3Dオブジェクト
    Skydorm.Draw();
    //human.Draw();
    box.Draw();
    //スプライト
    sprite.SpriteCommonBeginDraw(*spritePipelinePtr);
    debugText.Print(20, 20, 1, "Reset&RandomEndQ:SPACE");
    debugText.Print(20, 60, 1, "time:%d, mainQ: x.%f, y.%f, z.%f, w.%f",time,mainQ.x, mainQ.y, mainQ.z, mainQ.w);
    debugText.Print(20, 100, 1, "endQ: x.%f, y.%f, z.%f, w.%f",endQ.x, endQ.y, endQ.z, endQ.w);
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
    box.Updata(matView, matProjection,mainQ);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    depth = depthptr;
}

