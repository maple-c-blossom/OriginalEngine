#include "StageSelectScene.h"
#include "Scene.h"
#include "TitleScene.h"
#include "DemoScene.h"
#include "MiniatureGardenScene.h"

using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::StageSelectScene::SpriteInit()
{

    debugText_.Init(debugTextTexture_->texture.get());
    titleSprite_.CreateSprite();
	aButtonSprite_.CreateSprite();
	lStickSprite_.CreateSprite();
	stageSprite_[0].CreateSprite();
	stageSprite_[1].CreateSprite();
	stageSprite_[2].CreateSprite();
	stageSprite_[3].CreateSprite();
}

void MCB::StageSelectScene::ParticleInit()
{
}
unique_ptr<MCB::IScene> MCB::StageSelectScene::GetNextScene()
{
	PragmaPush
	PragmaWarningNum(5263)
    if (stages[selectStageNum] == "Demo")
    {
        unique_ptr<DemoScene> scene = move(make_unique<DemoScene>(rootparamsPtr_, depth_, pipeline_));
        return move(scene);
    }

	if ( stages[ selectStageNum ] == "Miniature" )
	{
		unique_ptr<MiniatureGardenScene> scene = move(make_unique<MiniatureGardenScene>(rootparamsPtr_,depth_,pipeline_));
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
	PragmaPop
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
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_B))
    {
        //soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }

    if (input_->IsKeyTrigger(DIK_UP) || (input_->gamePad_->LStick_.y_ >= 0.5f && selectMoveTime_.IsEnd()))
    {
        selectStageNum--;
        uint32_t time = selectMoveMaxTime_ + (selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime());
        selectMoveTime_.TimeSet(selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime(),static_cast<int32_t>(time));
        selectScaleTime_.Set(360);
    }

    if (input_->IsKeyTrigger(DIK_DOWN) || (input_->gamePad_->LStick_.y_ <= -0.5f && selectMoveTime_.IsEnd()))
    {
        selectStageNum++;
        uint32_t time = selectMoveMaxTime_ + (selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime());
        selectMoveTime_.TimeSet(selectMoveTime_.GetEndTime() - selectMoveTime_.NowTime(),static_cast< int32_t >( time));

    }
    selectStageNum = static_cast<uint32_t>(clamp(static_cast<float>(selectStageNum), 0.f,(static_cast< float >(stages.size()) - 1.f)));
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
        selectMoveStartPosy = dxWindow_->sWINDOW_CENTER_HEIGHT_ + ( static_cast<float>(( static_cast< int32_t >(i)
			- static_cast< int32_t >( oldSelectStageNum))) * 200.f);
		float temp = static_cast< float >( ( static_cast< int32_t >(i) - static_cast< int32_t >(selectStageNum)) * 200 );
        float posY = dxWindow_->sWINDOW_CENTER_HEIGHT_ + temp;

        posY = static_cast<float>(Lerp(static_cast<double>(selectMoveStartPosy), 
            static_cast<double>(posY), selectMoveTime_.GetEndTime(), selectMoveTime_.NowTime()));
		float size = 1;
        if (i == selectStageNum && selectMoveTime_.IsEnd())
        {
            float resultSize = sinf(ConvertRadius(static_cast<float>(selectScaleTime_.NowTime()) >= 180.f ? static_cast<float>(selectScaleTime_.NowTime()) * -1.f : static_cast<float>(selectScaleTime_.NowTime()))) * 0.25f + 1.2f;
            selectScaleTime_.SafeUpdate();
            selectScaleTime_.ReSet();
			size = resultSize;
        }

		stageSprite_[i].SpriteDraw(*stageTex_[i]->texture.get(),dxWindow_->sWINDOW_CENTER_WIDTH_,
		   posY,580 * size,64 * size);
    }
	aButtonSprite_.SpriteDraw(*abuttonTex_->texture.get(),170,
		dxWindow_->sWINDOW_HEIGHT_ - 150,580,64);
	lStickSprite_.SpriteDraw(*lStickTex_->texture.get(),180,
	dxWindow_->sWINDOW_HEIGHT_ - 50,580,64);
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
    stages[0] = "Game";
    stages[1] = "Demo";
    stages[2] = "Miniature";
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
	abuttonTex_ = loader_->LoadTexture(L"Resources\\Enter.png");
	lStickTex_ = loader_->LoadTexture(L"Resources\\SelectLS.png");
	stageTex_[0] = loader_->LoadTexture(L"Resources\\stageGame.png");
	stageTex_[1] = loader_->LoadTexture(L"Resources\\stageDemo.png");
	stageTex_[2] = loader_->LoadTexture(L"Resources\\Backyard.png");
	stageTex_[3] = loader_->LoadTexture(L"Resources\\stageTitle.png");
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
    ground_.rotation_ = { 0,0,0 };
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
