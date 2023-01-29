#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

MCB::Scene::~Scene()
{
    soundManager.AllDeleteSound();
    delete SpherModel;
    delete skydomeModel;
    delete groundModel;
    delete nextScene;
}

void MCB::Scene::Initialize()
{

    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
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
    ground.position = { 0,-3,0 };
    ;
    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };

    testSpher.Init();
    testSpher.model = SpherModel;
    testSpher.scale = { 1,1,1 };
    testSpher.position = { -4,-2,5 };
    testSpher.rotasion = { ConvertRadius(90),0,0 };


    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}


void MCB::Scene::LoadModel()
{
    SpherModel = new Model("sphere");
    SpherModel2 = new Model("sphere", true);

    groundModel = new Model("ground");

    skydomeModel = new Model("skydome");

    //testModel.Load("Resources\\testFbx\\boneTest.fbx");
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    zoomTex = loader->LoadTexture(L"Resources\\debugfont.png");
    scopeTex = loader->LoadTexture(L"Resources\\debugfont.png");

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
    debugText.Init(debugTextTexture->texture.get());

}

void MCB::Scene::ParticleInit()
{


    //testParticle.rotasion.x = ConvertRadius(-90);
}

IScene* MCB::Scene::GetNextScene()
{
    return new TitleScene(rootparamsPtr, depth, pipeline);
}



void MCB::Scene::Update()
{


    if (input->IsKeyDown(DIK_UP))
    {

    }
    if (input->IsKeyDown(DIK_DOWN))
    {
        matView.eye.y -= ConvertRadius(5);
    }

    if (input->IsKeyDown(DIK_LEFT))
    {
        matView.eye.x -= cos(ConvertRadius(5));
        matView.eye.z -= sin(ConvertRadius(5));
    }
    if (input->IsKeyDown(DIK_RIGHT))
    {
        matView.eye.x += cos(ConvertRadius(5));
        matView.eye.z += sin(ConvertRadius(5));
    }

    if (input->IsKeyTrigger(DIK_SPACE))
    {
        if (testSpher.model == SpherModel)
        {
            testSpher.model = groundModel;
        }
        else
        {
            testSpher.model = SpherModel;
        }
    }

    if (input->IsKeyDown(DIK_A))
    {
        testSpher.position.x -= 1;
    }
    if (input->IsKeyDown(DIK_D))
    {
        testSpher.position.x += 1;
    }

    if (input->IsKeyTrigger(DIK_1))
    {
        lights->SetPLightIsActive(0, false);
        lights->SetSLightIsActive(0, false);
        lights->SetDirLightIsActive(0, true);
    }
    else if (input->IsKeyTrigger(DIK_2))
    {
        lights->SetPLightIsActive(0, true);
        lights->SetSLightIsActive(0, false);
        lights->SetDirLightIsActive(0, false);
    }
    else if (input->IsKeyTrigger(DIK_3))
    {
        lights->SetPLightIsActive(0, false);
        lights->SetSLightIsActive(0, true);
        lights->SetDirLightIsActive(0, false);
    }
    if (input->IsKeyTrigger(DIK_5))
    {
        testSpher.model = SpherModel;
    }
    else if (input->IsKeyTrigger(DIK_6))
    {
        testSpher.model = SpherModel2;
    }
    lights->UpDate();

    MatrixUpdate();

    if (input->IsKeyTrigger(DIK_SPACE))
    {
        sceneEnd = true;
    }
}

void MCB::Scene::Draw()
{

    Skydorm.Draw();
    ground.Draw();

    testSpher.Draw();

}

void MCB::Scene::SpriteDraw()
{
    debugText.Print(20, 20, 1, "SPACE:SceneChange Arrow:CameraMove");
    debugText.Print(20, 40, 1, "1or2or3 LightChenge 5or6 smooth");
    debugText.Print(20, 60, 1, "AD:spherMove");
    debugText.AllDraw();
}

void MCB::Scene::ParticleDraw()
{

}

void MCB::Scene::CheckAllColision()
{
}


void MCB::Scene::ImGuiUpdate()
{
    imgui.Begin();
    //ImGui::ShowDemoWindow();
    if (ImGui::CollapsingHeader("debug"))
    {
    }


    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView(ybill);
    Skydorm.Update(matView, matProjection);
    ground.Update(matView, matProjection);
    testSpher.Update(matView, matProjection, false);

    matView.UpDateMatrixView(true);

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}
