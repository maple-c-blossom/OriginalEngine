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

    testAnimation.Init();
    testAnimation.model = SpherModel;
    testAnimation.scale = { 1,1,1 };
    testAnimation.position = { -4,-2,5 };
    testAnimation.rotasion = { ConvertRadius(90),0,0 };


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

    testAnimation.rotasion.y += 0.15f;
    if (input->IsKeyDown(DIK_UP))
    {
        lights->SetPLightPos(0, { lights->GetPLightPos(0).x,lights->GetPLightPos(0).y,lights->GetPLightPos(0).z + 1 });
        lights->SetSLightPos(0, { lights->GetSLightPos(0).x,lights->GetSLightPos(0).y,lights->GetSLightPos(0).z + 1 });
    }
    if (input->IsKeyDown(DIK_DOWN))
    {
        lights->SetPLightPos(0, { lights->GetPLightPos(0).x,lights->GetPLightPos(0).y,lights->GetPLightPos(0).z - 1 });
        lights->SetSLightPos(0, { lights->GetSLightPos(0).x,lights->GetSLightPos(0).y,lights->GetSLightPos(0).z - 1 });
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
        if (testAnimation.model == SpherModel)
        {
            testAnimation.model = groundModel;
        }
        else
        {
            testAnimation.model = SpherModel;
        }
    }

    if (input->IsKeyDown(DIK_A))
    {
        testAnimation.position.x -= 1;
    }
    if (input->IsKeyDown(DIK_D))
    {
        testAnimation.position.x += 1;
    }

    if (input->IsKeyTrigger(DIK_1))
    {
        lights->SetDirLightIsActive(0, !lights->GetDirLightIsActive(0));
    }
    else if (input->IsKeyTrigger(DIK_2))
    {
        lights->SetPLightIsActive(0, !lights->GetPLightIsActive(0));
    }
    else if (input->IsKeyTrigger(DIK_3))
    {
        lights->SetSLightIsActive(0, !lights->GetSLightIsActive(0));
    }

    if (input->IsKeyTrigger(DIK_5))
    {
        testAnimation.model = SpherModel;
    }
    else if (input->IsKeyTrigger(DIK_6))
    {
        testAnimation.model = SpherModel2;
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

    testAnimation.Draw();

}

void MCB::Scene::SpriteDraw()
{

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
    if (ImGui::CollapsingHeader("Infomation"))
    {
        if (ImGui::TreeNode("operation"))
        {
            ImGui::Text("SPACE:SceneChange UpOrDown LightMove RightOrLeft : CameraMove");
            ImGui::Text("1or2or3 LightChenge(1:Dir 2:Point 3:Spot) 5or6 smooth(5:NoSmooth 6:Smooth)");
            ImGui::Text("AD:spherMove");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Point"))
        {
            ImGui::Text("1:Phong reflection model");
            ImGui::Text("2:Smoothing");
            ImGui::Text("3:Light data separation");
            ImGui::Text("4:Phong shading ");
            ImGui::Text("5:MultipleLight ");
            ImGui::Text("6:PointLight ");
            ImGui::Text("7:SpotLight");
            ImGui::Text("8:SceneChenge");
            ImGui::Text("9:AsynchronousLoad");

            ImGui::TreePop();
        }
    }
    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView(ybill);
    Skydorm.Update(matView, matProjection);
    ground.Update(matView, matProjection);
    testAnimation.Update(matView, matProjection, false);

    matView.UpDateMatrixView(true);

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}
