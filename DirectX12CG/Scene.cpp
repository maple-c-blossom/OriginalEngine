#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"

using namespace MCB;
using namespace DirectX;

MCB::Scene::~Scene()
{
    soundManager.AllDeleteSound();
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;
    delete nextScene;
    delete testModel;
    //loader->SetDelete(testTex);
    //loader->SetDelete(debugTextTexture);
    //loader->SetDelete(zoomTex);
    //loader->SetDelete(scopeTex);
    testTex->free = true;
    debugTextTexture->free = true;
    zoomTex->free = true;
    scopeTex->free = true;

}

#pragma region 通常変数の初期化と3Dオブジェクトの初期化
void MCB::Scene::Initialize()
{
    debugCamera.Inilialize();
    viewCamera = &debugCamera;
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
    ground.position = { 0,0,0 };
    ;
    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };

    testSpher.Init();
    testSpher.model = BoxModel;
    testSpher.fbxModel = testModel;
    testSpher.scale = {1,1,1};
    testSpher.position = { 0,4,10 };
    testSpher.rotasion = { 0,0,0 };


    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{
	BoxModel = new Model("hoge");

	groundModel = new Model("ground");

	skydomeModel = new Model("skydome");
    testModel = new FBXModel();
    //testModel->Load("flash");
    testModel->Load("simple","gltf");
    //assert(testModel->textureManager->textures.size() < 20);
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
	testTex = loader->LoadTexture(L"Resources\\reimu.png");
	debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    zoomTex = loader->LoadTexture(L"Resources\\reticle.png");
    scopeTex = loader->CreateNoTextureFileIsTexture();

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
    testParticle.Init(testTex);
    testParticle.position = { 0,0,10 };
    //testParticle.rotasion.x = ConvertRadius(-90);
}

IScene* MCB::Scene::GetNextScene()
{
    return new TitleScene(rootparamsPtr,depth,pipeline);
}

#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
    Float3 SLPos = lights->GetSLightPos(0);
    Float3 PLPos = lights->GetPLightPos(0);


    if (input->IsKeyTrigger(DIK_SPACE))
    {
        //if (testSpher.model == BoxModel)
        //{
        //    testSpher.model = groundModel;
        //}
        //else
        //{
        //    testSpher.model = BoxModel;
        //}
        sceneEnd = true;
    }

    if (input->IsKeyDown(DIK_A))
    {
        testSpher.position.x += 1;
    }
    if (input->IsKeyDown(DIK_D))
    {
        testSpher.position.x -= 1;
    }

    testRotaPos[0].SetRota({ 0.71f,0.71f,0.0f }, 0.3f);
    testRotaPos[1] = { -testRotaPos[0].x,-testRotaPos[0].y,-testRotaPos[0].z,-testRotaPos[0].w};
    testRota[0] = testRota[0].Slerp(testRotaPos[0], testRotaPos[1], 0.0f);
    testRota[1] = testRota[1].Slerp(testRotaPos[0], testRotaPos[1], 0.3f);
    testRota[2] = testRota[2].Slerp(testRotaPos[0], testRotaPos[1], 0.5f);
    testRota[3] = testRota[3].Slerp(testRotaPos[0], testRotaPos[1], 0.7f);
    testRota[4] = testRota[4].Slerp(testRotaPos[0], testRotaPos[1], 1.0f);



        lights->UpDate();
        viewCamera->Update();
    //行列変換
    MatrixUpdate();
}

void MCB::Scene::Draw()
{
    //3Dオブジェクト
    Skydorm.Draw();
    ground.Draw();
    //human.Draw();
    //testSpher.Draw();
    pipeline->SetFbxPipeLine();//FBX用のシェーダにすると穴ぼこになる
    testSpher.FbxDraw();
    pipeline->SetObjPipeLine();
}

void MCB::Scene::SpriteDraw()
{
    debugText.Print(20, 20, 1, "hogehoge");
    debugText.AllDraw();
}

void MCB::Scene::ParticleDraw()
{
    testParticle.Draw();
}

void MCB::Scene::CheckAllColision()
{
}

void MCB::Scene::ImGuiUpdate()
{
    imgui.Begin();
    //ImGui::ShowDemoWindow();
    if (ImGui::CollapsingHeader("debug"))
    {
        ImGui::Text("testRota[0]:%0.2f,%0.2f,%0.2f,%0.2f", testRota[0].x, testRota[0].y, testRota[0].z, testRota[0].w);
        ImGui::Text("testRota[1]:%0.2f,%0.2f,%0.2f,%0.2f", testRota[1].x, testRota[1].y, testRota[1].z, testRota[1].w);
        ImGui::Text("testRota[2]:%0.2f,%0.2f,%0.2f,%0.2f", testRota[2].x, testRota[2].y, testRota[2].z, testRota[2].w);
        ImGui::Text("testRota[3]:%0.2f,%0.2f,%0.2f,%0.2f", testRota[3].x, testRota[3].y, testRota[3].z, testRota[3].w);
        ImGui::Text("testRota[4]:%0.2f,%0.2f,%0.2f,%0.2f", testRota[4].x, testRota[4].y, testRota[4].z, testRota[4].w);
    }


    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView(ybill);
    viewCamera->MatrixUpdate();
    Skydorm.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    ground.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    ground.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    testSpher.FbxUpdate(*viewCamera->GetView(), *viewCamera->GetProjection(),false);
    testParticle.Update(*viewCamera->GetView(), *viewCamera->GetProjection(), true);
    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr,PipeLineManager* pipeline)
{
	rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}

