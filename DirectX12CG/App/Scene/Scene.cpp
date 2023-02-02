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

}

void MCB::Scene::Object3DInit()
{

    ground;
    ground.Init();
    ground.model = groundModel.get();
    ground.scale = { 4,4,4 };
    ground.position = { 0,-3,0 };
    ground.SetCollider(new PlaneCollider({0,1,0},-3));

    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel.get();
    Skydorm.scale = { 4,4,4 };

    testAnimation.Init();
    testAnimation.model = SpherModel.get();
    testAnimation.scale = { 1,1,1 };
    testAnimation.position = { -4,-2,5 };
    testAnimation.rotasion = { ConvertRadius(90),0,0 };
    testAnimation.SetCollider(new MeshCollider);

    testRay.Init();
    testRay.model = SpherModel.get();
    testRay.scale = { 0.005f,0.005f,30 };
    testRay.position = { 0,0,0 };
    testRay.rotasion = { 0,0,0 };
    testRay.SetCollider(new RayCollider({0,0,0},{0,0,1}));

    triangle.triangle.SetCollider(new TriangleCollider({0,0,0},{0,0,-1},{triangle.PointA,triangle.PointB,triangle.PointC}));
    
   
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
    zoomTex = loader->LoadTexture(L"Resources\\testenemy.png");


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

 


    if (input->IsKeyDown(DIK_A))
    {
        testAnimation.position.x -= 0.1f;
    }
    if (input->IsKeyDown(DIK_D))
    {
        testAnimation.position.x += 0.1f;
    }
    if (input->IsKeyDown(DIK_W))
    {
        testAnimation.position.y += 0.1f;
    }
    if (input->IsKeyDown(DIK_S))
    {
        testAnimation.position.y -= 0.1f;
    }

    if (input->IsKeyDown(DIK_J))
    {
        testRay.position.x -= 0.1f;
    }
    if (input->IsKeyDown(DIK_L))
    {
        testRay.position.x += 0.1f;
    }
    if (input->IsKeyDown(DIK_K))
    {
        testRay.position.y += 0.1f;
    }
    if (input->IsKeyDown(DIK_I))
    {
        testRay.position.y -= 0.1f;
    }

    if (input->IsKeyDown(DIK_Z))
    {
        testAnimation.position.z += 1;
    }
    if (input->IsKeyDown(DIK_X))
    {
        testAnimation.position.z -= 1;
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
        testAnimation.model = SpherModel.get();
    }
    else if (input->IsKeyTrigger(DIK_6))
    {
        testAnimation.model = SpherModel2.get();
    }

    if (input->gamePad->IsButtonTrigger(GAMEPAD_B))
    {
        soundManager.PlaySoundWave(testSound, input->gamePad->IsButtonDown(GAMEPAD_LB));
    }

    if (input->gamePad->IsButtonTrigger(GAMEPAD_X))
    {
        soundManager.StopSoundWave(testSound);
        soundManager.StopSoundWave(test2Sound);
    }

    if (input->gamePad->IsButtonTrigger(GAMEPAD_Y))
    {
        soundManager.PlaySoundWave(test2Sound, input->gamePad->IsButtonDown(GAMEPAD_LB));
    }
    if (input->gamePad->LStick.y)
    {
        volume += (int)(input->gamePad->LStick.y * 4);
        if (volume < 0)
        {
            volume = 0;
        }
        else if (volume > 255)
        {
            volume = 255;
        }
        soundManager.SetVolume(volume, test2Sound);
    }

    lights->UpDate();



    MatrixUpdate();
    CollisionManager::GetInstance()->CheckAllCollision();
    if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
    {
        sceneEnd = true;
    }
}

void MCB::Scene::Draw()
{

    Skydorm.Draw();
    ground.Draw();
    triangle.DrawTriangle(viewCamera);
    testAnimation.Draw();
    testRay.Draw();

}

void MCB::Scene::SpriteDraw()
{
    sprite.SpriteDraw(*zoomTex->texture.get(), 500, 100);
    debugText.Print(300, 300,2, "hogehoge");
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
            ImGui::Text("SceneChange: [SPACE] or [GamePad A]");
            ImGui::Text("LightMove: [W],[S]");
            ImGui::Text("CameraMove: [ArrowKey],[N].[M]");
            ImGui::Text("CameraRota:[LSHIFT] + [Mouse LEFTClick] + [MouseMove]");
            ImGui::Text("LightChenge:[1 (Dir)] or [2 (Point)] or [3 (Spot)] ");
            ImGui::Text("LightActive:Dir = %s,Point = %s, Spot = %s",lights->GetDirLightIsActive(0) ? "true":"false", lights->GetPLightIsActive(0) ? "true" : "false", lights->GetSLightIsActive(0) ? "true" : "false");
            ImGui::Text("SmoothChange:[5 (NoSmooth)] or [6 (Smooth)]");
            ImGui::Text("SpherMove:[A],[D]");
            ImGui::Text("SoundPlay:[GamePad B] or [GamePad Y]");
            ImGui::Text("SoundLoopPlay:SoundPlayButtom + [GamePad LB]");
            ImGui::Text("SoundStop:[GamePad X]");
            ImGui::Text("LoopSoundVolume:[GamePad LStick] {Volume = %d (Max:255,Min:0)}",volume);


            ImGui::TreePop();
        }
    }
    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera->Update();
    Skydorm.Update(viewCamera);
    ground.Update(viewCamera);
    testAnimation.Update(viewCamera, false);
    testRay.Update(viewCamera, false);


    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}
