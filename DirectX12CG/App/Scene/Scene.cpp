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

    ground_.Init();
    ground_.model_ = groundModel_.get();
    ground_.scale_ = { 1,1,1 };
    ground_.position_ = { 0,-3,0 };
    ground_.rotasion_ = { 0,0,ConvertRadius(5)};
    ground_.SetCollider(make_shared<PlaneCollider>(Vector3D(0,1,0),-3.f));
    ground_.camera_ = viewCamera_;


    Skydorm_;
    Skydorm_.Init();
    Skydorm_.model_ = skydomeModel_.get();
    Skydorm_.scale_ = { 4,4,4 };
    Skydorm_.camera_ = viewCamera_;

    testsphere_.Init();
    testsphere_.model_ = SpherModel2_.get();
    testsphere_.scale_ = { 1,1,1 };
    testsphere_.position_ = { 0,0,3 };
    testsphere_.rotasion_ = { ConvertRadius(90),0,0 };
    testsphere_.SetCollider(move(make_shared<SphereCollider>()));
    testsphere_.camera_ = viewCamera_;
   
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}


void MCB::Scene::LoadModel()
{
    SpherModel_ = std::make_unique<Model>("sphere");
    SpherModel2_ = std::make_unique<Model>("sphere", true);

    groundModel_ = std::make_unique<Model>("ground");

    skydomeModel_ = std::make_unique<Model>("skydome");

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
    testSound_ = soundManager_.LoadWaveSound("Resources\\fanfare.wav");
    test2Sound_ = soundManager_.LoadWaveSound("Resources\\cat1.wav");
    soundManager_.SetVolume(100, testSound_);
    volume_ = 100;
    soundManager_.SetVolume(volume_, test2Sound_);
}

void MCB::Scene::SpriteInit()
{
    //postEffect->tex = debugTextTexture;
    debugText_.Init(debugTextTexture_->texture.get());

}

void MCB::Scene::ParticleInit()
{


    //testParticle.rotasion.x = ConvertRadius(-90);
}


unique_ptr<IScene> MCB::Scene::GetNextScene()
{
    return make_unique<TitleScene>(rootparamsPtr_, depth_, pipeline_);
}



void MCB::Scene::Update()
{



    //testsphere.rotasion.y += 0.05f;
    if (input_->IsKeyDown(DIK_W))
    {
        lights_->SetPLightPos(0, { lights_->GetPLightPos(0).x_,lights_->GetPLightPos(0).y_,
            lights_->GetPLightPos(0).z_ + 1 });
        lights_->SetSLightPos(0, { lights_->GetSLightPos(0).x_,lights_->GetSLightPos(0).y_,
lights_->GetSLightPos(0).z_ + 1 });
    }
    if (input_->IsKeyDown(DIK_S))
    {
        lights_->SetPLightPos(0, { lights_->GetPLightPos(0).x_,lights_->GetPLightPos(0).y_,
lights_->GetPLightPos(0).z_ - 1 });
        lights_->SetSLightPos(0, { lights_->GetSLightPos(0).x_,lights_->GetSLightPos(0).y_,
lights_->GetSLightPos(0).z_ - 1 });
    }



    if (input_->IsKeyDown(DIK_W))
    {
        testsphere_.position_.z += 0.1f;
    }
    if (input_->IsKeyDown(DIK_S))
    {
        testsphere_.position_.z -= 0.1f;
    }
    
    if (input_->IsKeyDown(DIK_D))
    {
        testsphere_.position_.x += 0.1f;
    }
    if (input_->IsKeyDown(DIK_A))
    {   
        testsphere_.position_.x -= 0.1f;
    }

    if (input_->IsKeyTrigger(DIK_T))
    {
        lights_->SetDirLightIsActive(0, !lights_->GetDirLightIsActive(0));
    }
    else if (input_->IsKeyTrigger(DIK_Y))
    {
        lights_->SetPLightIsActive(0, !lights_->GetPLightIsActive(0));
    }
    else if (input_->IsKeyTrigger(DIK_U))
    {
        lights_->SetSLightIsActive(0, !lights_->GetSLightIsActive(0));
    }



    if (input_->IsKeyTrigger(DIK_1))
    {
        testsphere_.shaderNum_ = 1.f;
    }
    else if (input_->IsKeyTrigger(DIK_2))
    {
        testsphere_.shaderNum_ = 2.f;
    }
    else if (input_->IsKeyTrigger(DIK_3))
    {
        testsphere_.shaderNum_ = 3.f;
    }

    if (input_->IsKeyTrigger(DIK_O))
    {
        testsphere_.model_ = SpherModel_.get();
    }
    else if (input_->IsKeyTrigger(DIK_P))
    {
        testsphere_.model_ = SpherModel2_.get();
    }
    
    lights_->UpDate();


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
    Skydorm_.Draw();
    ground_.Draw();
    testsphere_.Draw(zoomTex_->texture.get()->incrementNum_);
    postEffect_->PostDraw();


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
            ImGui::Text("ObjectShaderChange: [num 1 = Phone], [num 2 = Toon],[num3 = rimLight]");
            ImGui::Text("LightChenge:[T (Dir)] or [Y (Point)] or [U (Spot)] ");
            ImGui::Text("LightActive:Dir = %s,Point = %s, Spot = %s"
                ,lights_->GetDirLightIsActive(0) ? "true":"false", lights_->GetPLightIsActive(0) ? "true" : "false",
                lights_->GetSLightIsActive(0) ? "true" : "false");
            //ImGui::Text("SceneChange: [ENTER] or [GamePad A]");
            ImGui::Text("LightMove: [W],[S]");
            ImGui::Text("CameraMove: [ArrowKey],[N].[M]");
            ImGui::Text("CameraRota:[LSHIFT] + [Mouse LEFTClick] + [MouseMove]");
            ImGui::Text("SmoothChange:[O (NoSmooth)] or [P (Smooth)]");
            ImGui::Text("%f,%f,%f,", viewCamera_->GetView()->eye_.x, viewCamera_->GetView()->eye_.y,
                viewCamera_->GetView()->eye_.z);
            ImGui::Text("%f,%f,%f,", testsphere_.position_.x, testsphere_.position_.y, testsphere_.position_.z);

            ImGui::TreePop();
        }
    }
    imgui_.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera_->Update();
    Skydorm_.Update();
    ground_.Update();
    testsphere_.Update(false);
    

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr_ = root;
    depth_ = depthptr;
    pipeline_ = pipeline;
}
