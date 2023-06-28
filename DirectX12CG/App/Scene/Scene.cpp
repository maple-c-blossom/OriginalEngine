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
    loader_->Erase();
}

void MCB::Scene::Initialize()
{

    debugCamera_.Inilialize();
    viewCamera_ = &debugCamera_;
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
    player_.model_ = SpherModel_;
    player_.camera_ = viewCamera_;
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}


void MCB::Scene::LoadModel()
{

   SpherModel_ = modelManager_->GetModel("sphere", true);
   groundModel_ = modelManager_->GetModel("ground");
   skydomeModel_ = modelManager_->GetModel("skydome");
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

    player_.UniqueUpdate();
    level_->Update();
    lights_->UpDate();
    debugCamera_.Update();

    MatrixUpdate();
    
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
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
    postEffect_->PostDraw();
    player_.Draw();

}

void MCB::Scene::SpriteDraw()
{
    pipeline_->SetPostEffectPipeLine();
    postEffect_->Draw();

    pipeline_->SetSpritePipeLine();

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
}


void MCB::Scene::ImGuiUpdate()
{
    imgui_.Begin();
    //ImGui::ShowDemoWindow();
    if (ImGui::CollapsingHeader("Infomation"))
    {
        if (ImGui::TreeNode("operation"))
        {
            ImGui::TreePop();
        }
    }
    imgui_.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera_->Update();
    level_->UpdateMatrix();
    player_.Update();

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr_ = root;
    depth_ = depthptr;
    pipeline_ = pipeline;
}
