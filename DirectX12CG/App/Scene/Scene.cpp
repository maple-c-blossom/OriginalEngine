#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

MCB::Scene::~Scene()
{
    //soundManager_.AllDeleteSound();
    zoomTex_->free = true;
    debugTextTexture_->free = true;
    //modelManager_->erase();
    loader_->Erase();
}

void MCB::Scene::Initialize()
{

    maincamera_.Inilialize();
    debugCamera_.Inilialize();
    viewCamera_ = &maincamera_;
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    ParticleInit();
    //soundManager.PlaySoundWave(testSound, loopFlag);
    lights_->DefaultLightSet();
    lights_->UpDate();
    Object3d::SetLights(lights_);
    postEffect_->Init();
    postEffect_->color_.x_ = static_cast<float>(PostEffectNum::NONE);
    maincamera_.SetCameraTarget(&player_);
}

void MCB::Scene::Object3DInit()
{

    //ground_.Init();
    //ground_.model_ = groundModel_;
    //ground_.scale_ = { 1,1,1 };
    //ground_.position_ = { 0,-3,0 };
    //ground_.rotation_ = { 0,0,0};
    //ground_.SetCollider(move(make_unique<PlaneCollider>(Vector3D(0,1,0),-3.f)));
    //ground_.camera_ = viewCamera_;


    //Skydorm_;
    //Skydorm_.Init();
    //Skydorm_.model_ = skydomeModel_;
    //Skydorm_.scale_ = { 4,4,4 };
    //Skydorm_.camera_ = viewCamera_;

    //testsphere_.Init();
    //testsphere_.model_ = SpherModel_;
    //testsphere_.scale_ = { 1,1,1 };
    //testsphere_.position_ = { 0,0,3 };
    //testsphere_.rotation_ = { ConvertRadius(90),0,0 };
    //testsphere_.SetCollider(move(make_unique<SphereCollider>()));
    //testsphere_.camera_ = viewCamera_;
   
    level_ = move(LevelLoader::Load("testLevel",viewCamera_));

    player_.Init();
    player_.animationModel_ = playerModel_;
    player_.camera_ = viewCamera_;

    goal_.Init();
    goal_.model_ = SpherModel_;
    goal_.position_.y = 1;
    goal_.position_.z = 50;
    goal_.scale_ = { 3,3,3 };
    goal_.color_ = { 1,1,0,1 };
    goal_.popModel_ = goalModel_;
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}


void MCB::Scene::LoadModel()
{

   SpherModel_ = modelManager_->GetModel("sphere", true);
   //groundModel_ = modelManager_->GetModel("ground");
   //skydomeModel_ = modelManager_->GetModel("skydome");
   playerModel_ = modelManager_->GetModel("player", playerModel_);
   goalModel_ = modelManager_->GetModel("Boxtest");
    //testModel.Load("Resources\\testFbx\\boneTest.fbx");
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    zoomTex_ = loader_->LoadTexture(L"Resources\\09_Test_Texture2.jpg");
    //zoomTex = loader->LoadTexture(L"Resources\\testenemy.png");
    //zoomTex = loader->CreateNoTextureFileIsTexture();


}

void MCB::Scene::LoadSound()
{
    testSound_ = soundManager_->LoadWaveSound("Resources\\fanfare.wav");
    test2Sound_ = soundManager_->LoadWaveSound("Resources\\cat1.wav");
    soundManager_->SetVolume(100, testSound_);
    volume_ = 100;
    soundManager_->SetVolume(volume_, test2Sound_);
}

void MCB::Scene::SpriteInit()
{
    //postEffect->tex = debugTextTexture;
    debugText_.Init(debugTextTexture_->texture.get());

}

void MCB::Scene::ParticleInit()
{


    //testParticle.rotation.x = ConvertRadius(-90);
}


unique_ptr<IScene> MCB::Scene::GetNextScene()
{
    return move(make_unique<TitleScene>(rootparamsPtr_, depth_, pipeline_));
}



void MCB::Scene::Update()
{

    level_->Update();
    player_.UniqueUpdate();
    lights_->UpDate();
    debugCamera_.Update();
    maincamera_.Update();
    goal_.UpDate();
    //Node* node = playerModel_->ReadNode("LowerArm.R");
    //node->ikData.isIK = true;
    //node->ikData.iKTargetPosition = { player_.position_.x + 1.0f,player_.position_.y + 0.5f,player_.position_.z };
    CheckAllColision();
    MatrixUpdate();
    
    if (input_->IsKeyTrigger(DIK_LCONTROL))
    {
       //level_ = level_->ReLoad();
        Goal::ResetGoal();
        player_.position_ = { 0,0,-50 };
    }

    if (goal_.GetIsGoal() && (input_->IsKeyTrigger(DIK_SPACE) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A)))
    {
        sceneEnd_ = true;
    }
}

void MCB::Scene::Draw()
{
  

}

void MCB::Scene::PostEffectDraw()
{
    postEffect_->PreDraw();
    level_->Draw();
    goal_.Draw();

    pipeline_->SetFbxPipeLine();
    player_.AnimationDraw();
    postEffect_->PostDraw();

}

void MCB::Scene::SpriteDraw()
{
    pipeline_->SetPostEffectPipeLine();
    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();

    if (goal_.GetIsGoal())
    {
        debugText_.Print(dxWindow_->sWINDOW_WIDTH_ / 2, dxWindow_->sWINDOW_HEIGHT_ / 2, 5, "Goal!!!");
        debugText_.Print(dxWindow_->sWINDOW_WIDTH_ / 2, dxWindow_->sWINDOW_HEIGHT_ / 2 + 120, 3, "Press AButton");
    }
    debugText_.sprite_->color_ = { 1,1,1,1 };
    debugText_.Print(10, 10, 1, "Move:WASD or LStick");
    debugText_.Print(10, 30, 1, "(debug) Reset:LCONTROL");
    //postEffect->Draw();
 /*   sprite.SpriteDraw(*zoomTex->texture.get(), 500, 100);*/

    //debugText.Print(300, 300,2, "hogehoge");
    debugText_.AllDraw();
}

void MCB::Scene::ParticleDraw()
{
    
}

void MCB::Scene::CheckAllColision()
{
    CollisionManager::GetInstance()->CheckAllCollision();
}


void MCB::Scene::ImGuiUpdate()
{
    imgui_.Begin();
    ImGui::ShowDemoWindow();
    if (ImGui::CollapsingHeader("Infomation"))
    {
        if (ImGui::TreeNode("operation"))
        {
            ImGui::Text("LevelReLoad:LCONTROL");
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader("MotionModel"))
    {
            playerModel_->DrawHeirarchy();
            ImGui::TreePop();
    }
    imgui_.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera_->Update();
    level_->UpdateMatrix();
    player_.AnimationUpdate();
    player_.animationModel_->skeleton.JointObjectMatrixUpdate(viewCamera_, &player_, goalModel_,Float3(0.025f,0.025f,0.025f));
    goal_.Update();

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr_ = root;
    depth_ = depthptr;
    pipeline_ = pipeline;
}
