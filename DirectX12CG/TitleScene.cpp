#include "TitleScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{
    sprite.InitMatProje();
    sprite = sprite.CreateSprite();
    zoomSprite.InitMatProje();
    zoomSprite = zoomSprite.CreateSprite();
    scopeSprite.InitMatProje();
    scopeSprite = scopeSprite.CreateSprite();
    debugText.Init(&debugTextTexture);
}

MCB::IScene* MCB::TitleScene::GetNextScene()
{
	return new Scene(rootparamsPtr, depth, obj3dPipelinePtr, spritePipelinePtr,particlePipelinePtr);
}

void MCB::TitleScene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView();
    human.UpDate(matView, matProjection);
    Skydorm.Updata(matView, matProjection);
    ground.Updata(matView, matProjection);
    testSpher.Updata(matView, matProjection, true);
}

void MCB::TitleScene::Update()
{
    if (input->IsKeyTrigger(DIK_SPACE))
    {
        sceneEnd = true;
    }

    MatrixUpdate();
}

void MCB::TitleScene::Draw()
{
    //3Dオブジェクト
    Skydorm.Draw();
    ground.Draw();
    //human.Draw();
    testSpher.Draw();


}

void MCB::TitleScene::SpriteDraw()
{

    debugText.AllDraw();
}

void MCB::TitleScene::ParticleDraw()
{
}

MCB::TitleScene::TitleScene(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1, PipelineRootSignature* pipeline2)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    particlePipelinePtr = pipeline2;
	this->depth = depth;
}

MCB::TitleScene::~TitleScene()
{
    soundManager.AllDeleteSound();
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;
    delete nextScene;
}

void MCB::TitleScene::Initialize()
{
    matView.CreateMatrixView(XMFLOAT3(0.0f, 3.0f, -10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    //soundManager.PlaySoundWave(testSound, loopFlag);
    lights->DefaultLightSet();
    lights->UpDate();
    Object3d::SetLights(lights);
}

void MCB::TitleScene::LoadModel()
{
    BoxModel = new Model("sphere", true);

    groundModel = new Model("ground");

    skydomeModel = new Model("skydome");
}

void MCB::TitleScene::LoadTexture()
{
    testTex.CreateTexture(L"Resources\\reimu.png");
    debugTextTexture.CreateTexture(L"Resources\\debugfont.png");
    zoomTex.CreateTexture(L"Resources\\reticle.png");
    scopeTex.CreateTexture(L"Resources\\scope.png");
}

void MCB::TitleScene::LoadSound()
{
    testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::TitleScene::Object3DInit()
{
    human;
    human.Init(BoxModel);

    ground;
    ground.Init();
    ground.model = groundModel;
    ground.scale = { 4,4,4 };
    ground.position = { 0,0,0 };
    ;
    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };

    testSpher.Init();
    testSpher.model = BoxModel;
    testSpher.scale = { 3,3,3 };
    testSpher.position = { 0,4,10 };

}
