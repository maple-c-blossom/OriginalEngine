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
    testsphere.model = SpherModel.get();
    testsphere.scale = { 1,1,1 };
    testsphere.position = { -4,0,0 };
    testsphere.rotasion = { ConvertRadius(90),0,0 };
    testsphere.SetCollider(new SphereCollider);
    testsphere.camera = viewCamera;

    testsphere2.Init();
    testsphere2.model = SpherModel.get();
    testsphere2.scale = { 1,1,1 };
    testsphere2.position = { 0,0,0 };
    testsphere2.rotasion = { 0,0,0 };
    testsphere2.SetCollider(new SphereCollider);
    testsphere2.camera = viewCamera;

    testsphere3.Init();
    testsphere3.model = SpherModel.get();
    testsphere3.scale = { 1,1,1 };
    testsphere3.position = { -4,0,0 };
    testsphere3.rotasion = { 0,0,0 };
    testsphere3.SetCollider(new SphereCollider);
    testsphere3.camera = viewCamera;

    testsphere4.Init();
    testsphere4.model = SpherModel.get();
    testsphere4.scale = { 1,1,1 };
    testsphere4.position = { 0,3,0 };
    testsphere4.rotasion = { 0,0,0 };
    testsphere4.SetCollider(new SphereCollider);
    testsphere4.camera = viewCamera;

    testRay.Init();
    testRay.model = SpherModel.get();
    testRay.scale = { 0.02f,20,0.02f };
    testRay.position = { 0,0,0 };
    testRay.rotasion = { 0,0,0 };
    testRay.SetCollider(new RayCollider({0,+20,0},{0,-1,0}));
    testRay.camera = viewCamera;

    rayStart.Init();
    rayStart.model = SpherModel.get();
    rayStart.scale = { 1.0f,1,1.0f };
    rayStart.position = { 0,  testRay.position.y + 20,0 };
    rayStart.rotasion = { 0,0,0 };
    //rayStart.SetCollider(new RayCollider({ 0,+10,0 }, { 0,-1,0 }));
    rayStart.camera = viewCamera;

    triangle.triangle.SetCollider(new TriangleCollider({0,0,0},{0,0,-1},{triangle.PointA,triangle.PointB,triangle.PointC}));
    triangle.triangle.camera = viewCamera;
    triangle.triangle.position = { 0,3,0 };
   
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

 


    

    if (input->IsKeyDown(DIK_J))
    {
        testRay.position.x -= 0.1f;
    }
    if (input->IsKeyDown(DIK_L))
    {
        testRay.position.x += 0.1f;
    }

    if (isDown) { 
        testRay.position.y -= 0.1f; 
        if (testRay.position.y <= -30)
        {
            isDown = false;
        }
    }
    else {
        testRay.position.y += 0.1f;
        if (testRay.position.y >= 0)
        {
            isDown = true;
        }
    }
    if (!isRight) {
        testsphere4.position.x -= 0.15f;
        if (testsphere4.position.x <= 0)
        {
            isRight = true;
        }
    }
    else {
        testsphere4.position.x += 0.15f;
        if (testsphere4.position.x >= 6)
        {
            isRight = false;
        }
    }
    if (SphereisDown) {
        testsphere.position.y -= 0.05f;
        if (testsphere.position.y <= -5)
        {
            SphereisDown = false;
        }
    }
    else {
        testsphere.position.y += 0.05f;
        if (testsphere.position.y >= 0)
        {
            SphereisDown = true;
        }
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
        testsphere.model = SpherModel.get();
    }
    else if (input->IsKeyTrigger(DIK_6))
    {
        testsphere.model = SpherModel2.get();
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
    rayStart.position = { 0,  testRay.position.y + 20,0 };
    lights->UpDate();


    MatrixUpdate();
    if (!input->IsKeyDown(DIK_P))
    {
        RayCastHit info;
        Ray* rayA = dynamic_cast<Ray*>(testRay.GetCollider());
        if (CollisionManager::GetInstance()->Raycast(*rayA, &info))
        {
            info.objctPtr->OnCollision(CollisionInfomation(info.objctPtr, info.collPtr, info.inter));
        }
        else
        {

        }
    }
    CollisionManager::GetInstance()->CheckAllCollision();
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
    triangle.DrawTriangle(viewCamera);
    testsphere.Draw();
    testsphere2.Draw();
    testsphere3.Draw();
    testsphere4.Draw();
    testRay.Draw();
    rayStart.Draw();
    postEffect->PostDraw();
}

void MCB::Scene::SpriteDraw()
{
    postEffect->Draw();
    sprite.SpriteDraw(*zoomTex->texture.get(), 500, 100);
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
            ImGui::Text("SceneChange: [ENTER] or [GamePad A]");
            ImGui::Text("LightMove: [W],[S]");
            ImGui::Text("CameraMove: [ArrowKey],[N].[M]");
            ImGui::Text("CameraRota:[LSHIFT] + [Mouse LEFTClick] + [MouseMove]");
            ImGui::Text("LightChenge:[1 (Dir)] or [2 (Point)] or [3 (Spot)] ");
            ImGui::Text("LightActive:Dir = %s,Point = %s, Spot = %s",lights->GetDirLightIsActive(0) ? "true":"false", lights->GetPLightIsActive(0) ? "true" : "false", lights->GetSLightIsActive(0) ? "true" : "false");
            ImGui::Text("SmoothChange:[5 (NoSmooth)] or [6 (Smooth)]");
            ImGui::Text("RayCastOff[P]");
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
    Skydorm.Update();
    ground.Update();
    testsphere.Update(false);
    testsphere2.Update(false);
    testsphere3.Update(false);
    testsphere4.Update(false);
    testRay.Update(false);
    rayStart.Update(false);
    

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}
