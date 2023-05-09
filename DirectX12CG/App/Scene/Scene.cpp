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
    testsphere.position = { -4,0,0 };
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
    //zoomTex = loader->LoadTexture(L"Resources\\testenemy.png");
    zoomTex = loader->CreateNoTextureFileIsTexture();


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



    testsphere.rotasion.y += 0.15f;
    if (input->IsKeyDown(DIK_W))
    {
        lights->SetPLightPos(0, { lights->GetPLightPos(0).x,lights->GetPLightPos(0).y,lights->GetPLightPos(0).z + 1 });
        lights->SetSLightPos(0, { lights->GetSLightPos(0).x,lights->GetSLightPos(0).y,lights->GetSLightPos(0).z + 1 });
    }
    if (input->IsKeyDown(DIK_S))
    {
        lights->SetPLightPos(0, { lights->GetPLightPos(0).x,lights->GetPLightPos(0).y,lights->GetPLightPos(0).z - 1 });
        lights->SetSLightPos(0, { lights->GetSLightPos(0).x,lights->GetSLightPos(0).y,lights->GetSLightPos(0).z - 1 });
    }



    if (input->IsKeyDown(DIK_W))
    {
        testsphere.position.z += 0.1f;
    }
    if (input->IsKeyDown(DIK_S))
    {
        testsphere.position.z -= 0.1f;
    }
    
    if (input->IsKeyDown(DIK_D))
    {
        testsphere.position.x += 0.1f;
    }
    if (input->IsKeyDown(DIK_A))
    {   
        testsphere.position.x -= 0.1f;
    }

    if (input->IsKeyTrigger(DIK_T))
    {
        lights->SetDirLightIsActive(0, !lights->GetDirLightIsActive(0));
    }
    else if (input->IsKeyTrigger(DIK_Y))
    {
        lights->SetPLightIsActive(0, !lights->GetPLightIsActive(0));
    }
    else if (input->IsKeyTrigger(DIK_U))
    {
        lights->SetSLightIsActive(0, !lights->GetSLightIsActive(0));
    }

    if (input->IsKeyTrigger(DIK_1))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::STRIPE);
    }
    else if (input->IsKeyTrigger(DIK_2))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::AVARAGEBLER);
    }
    else if (input->IsKeyTrigger(DIK_3))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::INV);
    }
    else if (input->IsKeyTrigger(DIK_4))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::GAUSSIANBLUR);
    }
    else if (input->IsKeyTrigger(DIK_5))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::GRAY);
    }
    else if (input->IsKeyTrigger(DIK_6))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::SEPIA);
    }
    else if (input->IsKeyTrigger(DIK_0))
    {
        postEffect->color.x = static_cast<float>(PostEffectNum::NONE);
    }



    if (input->IsKeyTrigger(DIK_7))
    {
        testsphere.shaderNum = 1.f;
    }
    else if (input->IsKeyTrigger(DIK_8))
    {
        testsphere.shaderNum = 2.f;
    }
    else if (input->IsKeyTrigger(DIK_9))
    {
        testsphere.shaderNum = 3.f;
    }

    if (input->IsKeyTrigger(DIK_O))
    {
        testsphere.model = SpherModel.get();
    }
    else if (input->IsKeyTrigger(DIK_P))
    {
        testsphere.model = SpherModel2.get();
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
            ImGui::Text("PostEffectChange: [num 1 = stripe], [num 2 = avarageBler]");
            ImGui::Text("[num 3 = inv], [num 4 = GAUSSIANBLUR], [num 5 = gray]");
            ImGui::Text("[num 6 = sepia], [num 0 = NoEffect]");
            ImGui::Text("ObjectShadeChange: [num 7 = Phone], [num 8 = Toon],[num9 = rimLight]");
            ImGui::Text("LightChenge:[T (Dir)] or [Y (Point)] or [U (Spot)] ");
            ImGui::Text("LightActive:Dir = %s,Point = %s, Spot = %s",lights->GetDirLightIsActive(0) ? "true":"false", lights->GetPLightIsActive(0) ? "true" : "false", lights->GetSLightIsActive(0) ? "true" : "false");
            ImGui::Text("SceneChange: [ENTER] or [GamePad A]");
            ImGui::Text("LightMove: [W],[S]");
            ImGui::Text("CameraMove: [ArrowKey],[N].[M]");
            ImGui::Text("CameraRota:[LSHIFT] + [Mouse LEFTClick] + [MouseMove]");
            ImGui::Text("SmoothChange:[O (NoSmooth)] or [P (Smooth)]");


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
