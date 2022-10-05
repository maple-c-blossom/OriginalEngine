#include "Scene.h"
#include "TitleScene.h"
using namespace MCB;
using namespace DirectX;

MCB::Scene::~Scene()
{
    soundManager.AllDeleteSound();
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;
    delete nextScene;
}

#pragma region 通常変数の初期化と3Dオブジェクトの初期化
void MCB::Scene::Initialize()
{

    matView.CreateMatrixView(XMFLOAT3(0.0f, 3.0f, -10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    ParticleInit();
    //soundManager.PlaySoundWave(testSound, loopFlag);
    lights->DefaultLightSet();
    lights->UpDate();
    Object3d::SetLights(lights);

}

void MCB::Scene::Object3DInit()
{

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
    testSpher.scale = {3,3,3};
    testSpher.position = { 0,4,10 };



    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{
	BoxModel = new Model("box");

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

void MCB::Scene::ParticleInit()
{
    testParticle.Init(&scopeTex);
    testParticle.position = { 0,0,100 };
    testParticle.rotasion.x = ConvertRadius(-90);
}

IScene* MCB::Scene::GetNextScene()
{
    return new TitleScene(rootparamsPtr,depth,obj3dPipelinePtr,spritePipelinePtr,particlePipelinePtr);
}

#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
        //testSpher.rotasion.y += 0.01f;
        Float3 SLPos = lights->GetSLightPos(0);
        Float3 PLPos = lights->GetPLightPos(0);
        //if (input->IsKeyDown(DIK_W))
        //{
        //    SLPos.z += 0.01f;
        //}
        //if (input->IsKeyDown(DIK_S))
        //{
        //    SLPos.z -= 0.01f;
        //}

        //if (input->IsKeyDown(DIK_A))
        //{
        //    SLPos.y -= 0.01f;
        //}
        //if (input->IsKeyDown(DIK_D))
        //{
        //    SLPos.y += 0.01f;
        //}

        //if (input->IsKeyDown(DIK_UP))
        //{
        //    PLPos.z += 0.01f;
        //}
        //if (input->IsKeyDown(DIK_DOWN))
        //{
        //    PLPos.z -= 0.01f;
        //}

        //if (input->IsKeyDown(DIK_LEFT))
        //{
        //    PLPos.x -= 0.01f;
        //}
        //if (input->IsKeyDown(DIK_RIGHT))
        //{
        //    PLPos.x += 0.01f;
        //}

        if (input->IsKeyTrigger(DIK_SPACE))
        {
            sceneEnd = true;
        }


        if (input->IsKeyDown(DIK_UP))
        {
            matView.eye.y += ConvertRadius(1);
        }
        if (input->IsKeyDown(DIK_DOWN))
        {
            matView.eye.y -= ConvertRadius(1);
        }

        if (input->IsKeyDown(DIK_LEFT))
        {
            matView.eye.x -= ConvertRadius(1);
        }
        if (input->IsKeyDown(DIK_RIGHT))
        {
            matView.eye.x += ConvertRadius(1);
        }

        if (input->IsKeyTrigger(DIK_SPACE)) ybill = !ybill;

        matView.target = testSpher.position;
        lights->SetPLightPos(0, PLPos);
        lights->SetSLightPos(0, SLPos);

        lights->UpDate();
    //行列変換
    MatrixUpdate();
}

void MCB::Scene::Draw()
{

    //3Dオブジェクト
    Skydorm.Draw();
    ground.Draw();
    //human.Draw();
    testSpher.Draw();

}

void MCB::Scene::SpriteDraw()
{

    debugText.AllDraw();
}

void MCB::Scene::ParticleDraw()
{
    //testParticle.Draw();
}

void MCB::Scene::CheckAllColision()
{
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView(ybill);
    Skydorm.Updata(matView, matProjection);
    ground.Updata(matView, matProjection);
    testSpher.Updata(matView, matProjection,true);
    testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1, PipelineRootSignature* pipeline2)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    particlePipelinePtr = pipeline2;
    depth = depthptr;
}

