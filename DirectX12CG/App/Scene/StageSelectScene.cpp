#include "StageSelectScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::StageSelectScene::SpriteInit()
{

    debugText_.Init(debugTextTexture_->texture.get());
    titleSprite_.CreateSprite();
}

void MCB::StageSelectScene::ParticleInit()
{
}

unique_ptr<MCB::IScene> MCB::StageSelectScene::GetNextScene()
{
    unique_ptr<Scene> scene = move(make_unique<Scene>(rootparamsPtr_, depth_, pipeline_));
    scene->SetStage(stages[selectStageNum]);
    return move(scene);
}

void MCB::StageSelectScene::MatrixUpdate()
{
    //test2Animation_.UpdateMatrix();
    viewCamera_->Update();
    Skydorm_.Update();
    ground_.Update();
}

void MCB::StageSelectScene::Update()
{

    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        //soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }

    if (input_->IsKeyTrigger(DIK_UP) || input_->gamePad_->IsButtonTrigger(GAMEPAD_UP))
    {
        selectStageNum++;
    }

    if (input_->IsKeyTrigger(DIK_DOWN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_DOWN))
    {
        selectStageNum--;
    }
    selectStageNum = static_cast<int32_t>(clamp(static_cast<float>(selectStageNum), 0.f, stages.size() - 1.f));

    MatrixUpdate();
}

void MCB::StageSelectScene::PostEffectDraw()
{
    postEffect_->PreDraw();
    Skydorm_.Draw();
    ground_.Draw();
    postEffect_->PostDraw();
}

void MCB::StageSelectScene::Draw()
{
    //3Dオブジェクト


}

void MCB::StageSelectScene::SpriteDraw()
{

    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();
    //titleSprite_.SpriteDraw(*titleTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_);
    debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_ - 100, dxWindow_->sWINDOW_CENTER_HEIGHT_ + 100, 2, "Stage::%s",stages[selectStageNum].c_str());
    debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_ + 200, 2, "Press AButton");
    debugText_.AllDraw();
}

void MCB::StageSelectScene::ParticleDraw()
{
}

void MCB::StageSelectScene::CheckAllColision()
{
}

void MCB::StageSelectScene::ImGuiUpdate()
{
    imgui_.Begin();
    imgui_.End();
}

MCB::StageSelectScene::StageSelectScene(RootParameter* root, Depth* depth, PipeLineManager* pipeline)
{
    rootparamsPtr_ = root;
    depth_ = depth;
    pipeline_ = pipeline;
}

MCB::StageSelectScene::~StageSelectScene()
{
    soundManager_->AllDeleteSound();
    debugTextTexture_->free = true;

    //modelManager_->erase();
    loader_->Erase();
}

void MCB::StageSelectScene::Initialize()
{
    stages[0] = "testLevel";
    stages[1] = "testLevelCopy";
    camera_.Inilialize();
    viewCamera_ = &camera_;
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
}

void MCB::StageSelectScene::LoadModel()
{

    groundModel_ = std::make_unique<Model>("ground");

    skydomeModel_ = std::make_unique<Model>("skydome");

    sphereModel_ = std::make_unique<Model>("sphere");

}

void MCB::StageSelectScene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    titleTex_ = loader_->LoadTexture(L"Resources\\Title.png");
}

void MCB::StageSelectScene::LoadSound()
{
    selectSound_ = soundManager_->LoadWaveSound("Resources\\select.wav");
    test2Sound_ = soundManager_->LoadWaveSound("Resources\\fanfare.wav");
    soundManager_->SetVolume(100, selectSound_);
}

void MCB::StageSelectScene::Object3DInit()
{

    ground_.Init();
    ground_.model_ = groundModel_.get();
    ground_.scale_ = { 1,1,1 };
    ground_.position_ = { 0,-4,0 };
    ground_.rotation_ = { 0,0,ConvertRadius(5) };
    ground_.SetCollider(std::move(std::make_unique<MeshCollider>(groundModel_.get())));
    ground_.camera_ = viewCamera_;

    play_.Init();
    play_.model_ = sphereModel_.get();
    play_.position_ = { 0,3,0 };
    play_.camera_ = viewCamera_;


    Skydorm_.Init();
    Skydorm_.model_ = skydomeModel_.get();
    Skydorm_.scale_ = { 4,4,4 };
    Skydorm_.camera_ = viewCamera_;


}
