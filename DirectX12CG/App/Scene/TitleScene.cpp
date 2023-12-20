#include "TitleScene.h"
#include "StageSelectScene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{


    debugText_.Init(debugTextTexture_->texture.get());
    titleSprite_.CreateSprite();
	aButtonSprite_.CreateSprite();
}

void MCB::TitleScene::ParticleInit()
{
}

unique_ptr<MCB::IScene> MCB::TitleScene::GetNextScene()
{
	return move(make_unique<StageSelectScene>(rootparamsPtr_, depth_, pipeline_));
}

void MCB::TitleScene::MatrixUpdate()
{
    //test2Animation_.UpdateMatrix();
    viewCamera_->Update();

	Skydorm_.Update();
	ground_.Update();
}

void MCB::TitleScene::Update()
{

    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        //soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }
    
    MatrixUpdate();
}

void MCB::TitleScene::PostEffectDraw()
{
    postEffect_->PreDraw();
    //pipeline_->SetObjPipeLine(false, true);
	Skydorm_.Draw();
	ground_.Draw();
    pipeline_->SetObjPipeLine();
    postEffect_->PostDraw();
}

void MCB::TitleScene::Draw()
{
    //3Dオブジェクト


}

void MCB::TitleScene::SpriteDraw()
{
    float titleMove = sinf(ConvertRadius(static_cast<float>(titleMoveTime_.NowTime()) >= 180.f ? static_cast<float>(titleMoveTime_.NowTime()) * -1.f : static_cast<float>(titleMoveTime_.NowTime()))) * 10.f;
    titleMoveTime_.SafeUpdate();
    titleMoveTime_.ReSet();
    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();
    titleSprite_.SpriteDraw(*titleTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_,
        dxWindow_->sWINDOW_CENTER_HEIGHT_ + titleMove);
	aButtonSprite_.SpriteDraw(*abuttonTex_->texture.get(),dxWindow_->sWINDOW_CENTER_WIDTH_,
		dxWindow_->sWINDOW_HEIGHT_ - 180);
    debugText_.AllDraw();
}

void MCB::TitleScene::ParticleDraw()
{
}

void MCB::TitleScene::CheckAllColision()
{
}

void MCB::TitleScene::ImGuiUpdate()
{
    imgui_.Begin();
    imgui_.End();
}

MCB::TitleScene::TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline)
{
	rootparamsPtr_ = root;
	depth_ = depth;
    pipeline_ = pipeline;
}

MCB::TitleScene::~TitleScene()
{
    //soundManager_->AllDeleteSound();
    debugTextTexture_->free = true;

    //modelManager_->erase();
    loader_->Erase();
}

void MCB::TitleScene::Initialize()
{
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
    titleMoveTime_.Set(360);
}

void MCB::TitleScene::LoadModel()
{
	groundModel_ = std::make_unique<Model>("ground");

	skydomeModel_ = std::make_unique<Model>("skydome");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    backGroundTex_ = loader_->LoadTexture(L"Resources\\backGround.png");
    titleTex_ = loader_->LoadTexture(L"Resources\\Title.png");
    abuttonTex_ = loader_->LoadTexture(L"Resources\\PressEnter.png");
}

void MCB::TitleScene::LoadSound()
{
    selectSound_ = soundManager_->LoadWaveSound("Resources\\sounds\\select.wav");
    soundManager_->SetVolume(100, selectSound_);
}

void MCB::TitleScene::Object3DInit()
{

	ground_.Init();
	ground_.model_ = groundModel_.get();
	ground_.scale_ = { 1,1,1 };
	ground_.position_ = { 0,-4,0 };
	ground_.rotation_ = { 0,0,0 };
	ground_.SetCollider(std::move(std::make_unique<MeshCollider>(groundModel_.get())));
	ground_.camera_ = viewCamera_;

	Skydorm_.Init();
	Skydorm_.model_ = skydomeModel_.get();
	Skydorm_.scale_ = { 4,4,4 };
	Skydorm_.camera_ = viewCamera_;
   
}
