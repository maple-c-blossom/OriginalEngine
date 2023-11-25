#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

MCB::Scene::~Scene()
{
    //soundManager_.AllDeleteSound();
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
    maincamera_.SetCameraTarget(level_->GetObjectPtr("player"));
    startTimer.TimeSet(300,300);
    goal_ = dynamic_cast<Goal*>(level_->GetObjectPtr("goal"));
    player_ = dynamic_cast<Player*>(level_->GetObjectPtr("player"));
    player_->runFast = runFast;
    player_->runNormal = runNormal;
}

void MCB::Scene::SetStage(std::string stageName)
{
    stage = stageName;
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
   
    level_ = move(LevelLoader::Load(stage,viewCamera_));

    //player_.Init();
    //player_.animationModel_ = playerModel_;
    //player_.camera_ = viewCamera_;

    //goal_.Init(); 
    //goal_.model_ = SpherModel_;
    //goal_.position_.y = 1;
    //goal_.position_.z = 50;
    //goal_.scale_ = { 3,3,3 };
    //goal_.color_ = { 1,1,0,1 };
    //goal_.popModel_ = goalModel_;

    //check.Init();
    //check.model_ = SpherModel_;
    //check.position_.y = 1;
    //check.position_.z = 25;
    //check.scale_ = { 3,3,3 };
    //check.color_ = { 1,1,0,1 };
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
   goalModel_ = modelManager_->GetModel("star");
    //testModel.Load("Resources\\testFbx\\boneTest.fbx");
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    //zoomTex = loader->LoadTexture(L"Resources\\testenemy.png");
    //zoomTex = loader->CreateNoTextureFileIsTexture();


}

void MCB::Scene::LoadSound()
{
    runNormal = soundManager_->LoadWaveSound("Resources\\sounds\\runNormal.wav");
    runFast = soundManager_->LoadWaveSound("Resources\\sounds\\runFast.wav");
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
    startTimer.SafeDownUpdate();
    level_->Update(startTimer.NowTime() <= 0 && !goal_->GetIsGoal());
    if (goal_->GetIsGoal())
    {
        player_->currentAnimation_ = "Idle";
    }
    lights_->UpDate();
    debugCamera_.Update();
    maincamera_.Update();
    //goal_.UniqueUpdate();
    //Node* node = playerModel_->ReadNode("LowerArm.R");
    //node->ikData.isIK = true;
    //node->ikData.iKTargetPosition = { player_.position_.x + 1.0f,player_.position_.y + 0.5f,player_.position_.z };
    CheckAllColision();
    MatrixUpdate();
    

    if (goal_->GetIsGoal() && (input_->IsKeyTrigger(DIK_SPACE) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A)))
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
    level_->Draw(pipeline_);
    pipeline_->SetFbxPipeLine();
    level_->AnimationDraw();
	//playerModel_->skeleton.GetNode("mixamorig:LeftFoot")->jointView = true;
	pipeline_->SetObjPipeLine(false,false);
	player_->animationModel_->skeleton.JointObjectDraw();
	pipeline_->SetLinePipeLine();
	level_->GetObjectPtr("player")->animationModel_->skeleton.JointLineDraw();

    postEffect_->PostDraw();

}

void MCB::Scene::SpriteDraw()
{
    pipeline_->SetPostEffectPipeLine();
    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();

    debugText_.sprite_->color_ = { 1,1,1,1 };
    debugText_.Print(10, 10, 1, "accele:W or RTrriger");
    debugText_.Print(10, 30, 1, "brake:S or LTrriger");
    debugText_.Print(10, 60, 1, "Move:AD or LStick");

    if (!(startTimer.NowTime() <= 0))
    {
        int32_t time = startTimer.NowTime() / 60;
        if(time > 0) debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_, 3, "%d", time);
        else
        {
            debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_, 4, "START!!");
            if (!startDrawed)
            {
                startTimer.TimeSet(10, 0);
                startDrawed = true;
            }
        }
    }
    level_->DebugTextDraw(&debugText_);
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
    //ImGui::ShowDemoWindow();
    //if (ImGui::CollapsingHeader("Infomation"))
    //{
    //    if (ImGui::TreeNode("operation"))
    //    {
    //        ImGui::Text("LevelReLoad:LCONTROL");
    //        ImGui::TreePop();
    //    }
    //}

    if (ImGui::CollapsingHeader("MotionModel"))
    {
         playerModel_->DrawHeirarchy();
    }
	player_->Debug();
    imgui_.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera_->Update();
    level_->UpdateMatrix();
    //player_.AnimationUpdate();
	level_->GetObjectPtr("player")->animationModel_->skeleton.JointObjectMatrixUpdate(viewCamera_,
		level_->GetObjectPtr("player"),goalModel_);
    //goal_.Update();
    //check.Update();

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr_ = root;
    depth_ = depthptr;
    pipeline_ = pipeline;
}
