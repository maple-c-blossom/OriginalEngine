#include "StageSelectScene.h"
#include "Scene.h"
#include "TitleScene.h"
#include "DemoScene.h"
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
    if (stages[selectStageNum] == "Demo")
    {
        unique_ptr<DemoScene> scene = move(make_unique<DemoScene>(rootparamsPtr_, depth_, pipeline_));
        return move(scene);
    }

    if (stages[selectStageNum] == "Title")
    {
        unique_ptr<TitleScene> scene = move(make_unique<TitleScene>(rootparamsPtr_, depth_, pipeline_));
        return move(scene);
    }
    unique_ptr<Scene> scene = move(make_unique<Scene>(rootparamsPtr_, depth_, pipeline_));
    scene->SetStage(stages[selectStageNum]);
    return move(scene);
}

void MCB::StageSelectScene::MatrixUpdate()
{
    //test2Animation_.UpdateMatrix();
    //viewCamera_->Update();

    viewCamera_->GetView()->UpDateMatrixView();
    viewCamera_->GetProjection()->UpdataMatrixProjection();
    Skydorm_.Update();
    ground_.Update();
}

void MCB::StageSelectScene::Update()
{

    if(selectMoveTime_.IsEnd())oldSelectStageNum = selectStageNum;
    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        //soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }

    if (input_->IsKeyTrigger(DIK_UP) || (input_->gamePad_->LStick_.y_ >= 0.5f && selectMoveTime_.IsEnd()))
    {
        selectStageNum--;
        uint32_t time = selectMoveMaxTime_ + (selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime());
        selectMoveTime_.TimeSet(selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime(),time);
        selectScaleTime_.Set(360);
    }

    if (input_->IsKeyTrigger(DIK_DOWN) || (input_->gamePad_->LStick_.y_ <= -0.5f && selectMoveTime_.IsEnd()))
    {
        selectStageNum++;
        uint32_t time = selectMoveMaxTime_ + (selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime());
        selectMoveTime_.TimeSet(selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime(), time);
    }
    selectStageNum = static_cast<int32_t>(clamp(static_cast<float>(selectStageNum), 0.f, stages.size() - 1.f));
    selectMoveTime_.SafeUpdate();
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
    for (size_t i = 0; i < stages.size(); i++)
    {
        selectMoveStartPosy = dxWindow_->sWINDOW_CENTER_HEIGHT_ + (100.f * (i - oldSelectStageNum));
        float scale = 3;
        float selectMoveStartScale = scale - (abs(static_cast<float>(i) - oldSelectStageNum) / 2.f);
        scale = scale - (abs(static_cast<float>(i) - selectStageNum) / 2.f);
        scale = static_cast<float>(OutQuad(static_cast<double>(selectMoveStartScale),
            static_cast<double>(scale), selectMoveTime_.GetEndTime(), selectMoveTime_.NowTime()));

        float posY = dxWindow_->sWINDOW_CENTER_HEIGHT_ + (100.f * ((static_cast<int32_t>(i) - selectStageNum) * scale));
        posY = static_cast<float>(OutQuad(static_cast<double>(selectMoveStartPosy), 
            static_cast<double>(posY), selectMoveTime_.GetEndTime(), selectMoveTime_.NowTime()));

        //if (posY > dxWindow_->sWINDOW_CENTER_HEIGHT_ + 150)
        //{
        //    break;
        //}

        if (i == selectStageNum && selectMoveTime_.IsEnd())
        {
            float resultSize = sinf(ConvertRadius(static_cast<float>(selectScaleTime_.NowTime()) >= 180.f ? static_cast<float>(selectScaleTime_.NowTime()) * -1.f : static_cast<float>(selectScaleTime_.NowTime()))) * 0.25f + 1.2f;
            selectScaleTime_.SafeUpdate();
            selectScaleTime_.ReSet();
            scale = scale + resultSize / 2;
        }

        debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_ - 100, 
           posY , scale,
            "Stage::%s",stages[i].c_str());
    }
    debugText_.Print(50, dxWindow_->sWINDOW_HEIGHT_ - 100 , 2, "Enter: AButton");
    debugText_.Print(50, dxWindow_->sWINDOW_HEIGHT_ - 50 , 2, "Select: LStick");
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
    //soundManager_->AllDeleteSound();
    debugTextTexture_->free = true;

    //modelManager_->erase();
    loader_->Erase();
}

void MCB::StageSelectScene::Initialize()
{
    stages[0] = "Demo";
    stages[1] = "Tutorial";
    stages[2] = "Level1";
    stages[3] = "Title";
    camera_.Inilialize();
    camera_.moveStop = true;
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
    selectMoveTime_.TimeSet(1, 1);
    selectScaleTime_.Set(360);
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
    selectSound_ = soundManager_->LoadWaveSound("Resources\\sounds\\select.wav");
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
