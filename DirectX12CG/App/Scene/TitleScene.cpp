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
    debugText.Init(debugTextTexture->texture.get());
}

void MCB::TitleScene::ParticleInit()
{
}

MCB::IScene* MCB::TitleScene::GetNextScene()
{
	return new Scene(rootparamsPtr, depth, pipeline);
}

void MCB::TitleScene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView();
    Skydorm.Update(matView, matProjection);
    ground.Update(matView, matProjection);
    testAnimation.AnimationUpdate(matView, matProjection);
    test2Animation.AnimationUpdate(matView, matProjection);
}

void MCB::TitleScene::Update()
{
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
    lights->UpDate();
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
    pipeline->SetFbxPipeLine();
    testAnimation.AnimationDraw();
    test2Animation.AnimationDraw();
    pipeline->SetObjPipeLine();

}

void MCB::TitleScene::SpriteDraw()
{

    debugText.AllDraw();
}

void MCB::TitleScene::ParticleDraw()
{
}

void MCB::TitleScene::CheckAllColision()
{
}

void MCB::TitleScene::ImGuiUpdate()
{
    imgui.Begin();
    //ImGui::ShowDemoWindow();
    if (ImGui::CollapsingHeader("Infomation"))
    {
        if (ImGui::TreeNode("operation"))
        {
            ImGui::Text("SPACE:SceneChange UpOrDown LightMove RightOrLeft : CameraMove");
            ImGui::Text("1or2or3 LightChenge(1:Dir 2:Point 3:Spot) 5or6 smooth(5:NoSmooth 6:Smooth)");
            ImGui::Text("LightActive:Dir = %s,Point = %s, Spot = %s", lights->GetDirLightIsActive(0) ? "true" : "false", lights->GetPLightIsActive(0) ? "true" : "false", lights->GetSLightIsActive(0) ? "true" : "false");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Point"))
        {
            ImGui::Text("1:Animation");
            ImGui::TreePop();
        }
    }
    imgui.End();
}

MCB::TitleScene::TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline)
{
	rootparamsPtr = root;
	this->depth = depth;
    this->pipeline = pipeline;
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
    matView.CreateMatrixView(XMFLOAT3(0.0f, 3.0f, -100.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
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

void MCB::TitleScene::LoadModel()
{
    BoxModel = new Model("sphere", true);

    groundModel = new Model("ground");

    skydomeModel = new Model("skydome");

    animModel = std::make_unique<AnimationModel>();
    animModel->Load("gamewsdsa");

    anim2Model = std::make_unique<AnimationModel>();
    anim2Model->Load("testFbx");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    testTex = loader->LoadTexture(L"Resources\\reimu.png");
    zoomTex = loader->LoadTexture(L"Resources\\reticle.png");
    scopeTex = loader->LoadTexture(L"Resources\\scope.png");
}

void MCB::TitleScene::LoadSound()
{
    testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::TitleScene::Object3DInit()
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

    testAnimation.Init();
    //testAnimation.model = BoxModel;
    testAnimation.animationModel = animModel.get();
    testAnimation.scale = { 3,3,3 };
    testAnimation.position = { 0,4,10 };
    testAnimation.rotasion.y = ConvertRadius(90);
    
    test2Animation.animationModel = anim2Model.get();
    test2Animation.scale = { 3,3,3 };
    test2Animation.position = { 10,4,10 };
    test2Animation.rotasion.y = ConvertRadius(90);
}
