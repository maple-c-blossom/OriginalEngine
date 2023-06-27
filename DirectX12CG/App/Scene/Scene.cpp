#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

MCB::Scene::~Scene()
{
    soundManager.AllDeleteSound();
    zoomTex->free = true;
    debugTextTexture->free = true;
    delete nextScene;
    loader->Erase();
}

void MCB::Scene::Initialize()
{

    debugCamera.Inilialize();
    viewCamera = &debugCamera;
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
    postEffect->Init();
    postEffect->color.x = static_cast<float>(PostEffectNum::NONE);

}

void MCB::Scene::Object3DInit()
{

    ground.Init();
    ground.model = groundModel.get();
    ground.scale = { 1,1,1 };
    ground.position = { 0,-3,0 };
    ground.rotasion = { 0,0,ConvertRadius(5)};
    ground.SetCollider(new PlaneCollider{{0,1,0},-3});
    ground.camera = viewCamera;


    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel.get();
    Skydorm.scale = { 4,4,4 };
    Skydorm.camera = viewCamera;

    testsphere.Init();
    testsphere.model = SpherModel2.get();
    testsphere.scale = { 1,1,1 };
    testsphere.position = { 0,0,3 };
    testsphere.rotasion = { ConvertRadius(90),0,0 };
    testsphere.SetCollider(new SphereCollider);
    testsphere.camera = viewCamera;
   
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}


void MCB::Scene::LoadModel()
{
    SpherModel = std::make_unique<Model>("sphere");
    SpherModel2 = std::make_unique<Model>("sphere", true);

    groundModel = std::make_unique<Model>("ground");

    skydomeModel = std::make_unique<Model>("skydome");

    //testModel.Load("Resources\\testFbx\\boneTest.fbx");
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    zoomTex = loader->LoadTexture(L"Resources\\09_Test_Texture2.jpg");
    //zoomTex = loader->LoadTexture(L"Resources\\testenemy.png");
    //zoomTex = loader->CreateNoTextureFileIsTexture();


}

void MCB::Scene::LoadSound()
{
    testSound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    soundManager.SetVolume(100, testSound);
    volume = 100;
    soundManager.SetVolume(volume, test2Sound);
}

void MCB::Scene::SpriteInit()
{
    //postEffect->tex = debugTextTexture;
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



    
    if (input->IsKeyTrigger(DIK_SPACE))
    {
        blurFlag = !blurFlag;
    }
    if (blurFlag)
    {
        postEffect->color.x = 4;
    }
    else
    {
        postEffect->color.x = 7;
    }
    lights->UpDate();


    MatrixUpdate();
   
    if (input->IsKeyTrigger(DIK_RETURN) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
    {
        sceneEnd = true;
    }
}

void MCB::Scene::Draw()
{
  

}

void MCB::Scene::PostEffectDraw()
{
    postEffect->PreDraw();
    Skydorm.Draw();
    ground.Draw();
    testsphere.Draw(zoomTex->texture.get()->incrementNum);
    postEffect->PostDraw();


}

void MCB::Scene::SpriteDraw()
{
    pipeline->SetPostEffectPipeLine();
    postEffect->Draw();

    pipeline->SetSpritePipeLine();

    //postEffect->Draw();
 /*   sprite.SpriteDraw(*zoomTex->texture.get(), 500, 100);*/

    //debugText.Print(300, 300,2, "hogehoge");
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
            ImGui::Text("Blur:%s : Change[SPACE]",blurFlag ? "ON":"OFF");
            ImGui::TreePop();
        }
    }
    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera->Update();
    Skydorm.Update();
    ground.Update();
    testsphere.Update(false);
    

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}
