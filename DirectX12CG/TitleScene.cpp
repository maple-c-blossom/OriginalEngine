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
}

void MCB::TitleScene::Update()
{
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
            ImGui::Text("SPACE:SceneChange");
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

}
