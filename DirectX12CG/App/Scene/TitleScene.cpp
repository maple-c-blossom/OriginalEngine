#include "TitleScene.h"
#include "StageSelectScene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{


    debugText_.Init(debugTextTexture_->texture.get());
    titleSprite_.CreateSprite();
    backGroundSprite_.CreateSprite();
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
    test2Animation_.AnimationUpdate();
    testsphere_.Update();
    test2Animation_.animationModel_->skeleton.JointObjectMatrixUpdate(viewCamera_,
        &test2Animation_,boxModel_.get());
}

void MCB::TitleScene::Update()
{

    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }
    
    MatrixUpdate();
}

void MCB::TitleScene::PostEffectDraw()
{
    postEffect_->PreDraw();
    //pipeline_->SetObjPipeLine(false, true);
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
    backGroundSprite_.SpriteDraw(*backGroundTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_,
        dxWindow_->sWINDOW_CENTER_HEIGHT_ , dxWindow_->sWINDOW_WIDTH_, dxWindow_->sWINDOW_HEIGHT_);
    titleSprite_.SpriteDraw(*titleTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_,
        dxWindow_->sWINDOW_CENTER_HEIGHT_ + titleMove);
    debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_ + 200, 2, "Press AButton");
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

    sphereModel_ = std::make_unique<Model>("sphere");

    boxModel_ = std::make_unique<Model>("Bone");


    animModel_ = std::make_unique<AnimationModel>();
    animModel_->Load("gamewsdsa");

    anim2Model_ = std::make_unique<AnimationModel>();
    anim2Model_->Load("player");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    backGroundTex_ = loader_->LoadTexture(L"Resources\\backGround.png");
    titleTex_ = loader_->LoadTexture(L"Resources\\Title.png");
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

    testsphere_.Init();
    //testsphere.model = BoxModel;
    testsphere_.model_ = sphereModel_.get();
    testsphere_.scale_ = { 0.25f,0.25f,0.25f };
    testsphere_.position_ = { 5,1,0 };
    testsphere_.rotation_.y = ConvertRadius(90);
    testsphere_.camera_ = viewCamera_;

    test2Animation_.animationModel_ = anim2Model_.get();
    test2Animation_.scale_ = { 1,1,1 };
    test2Animation_.position_ = { 0,0,0 };
    test2Animation_.camera_ = viewCamera_;

    poleVec = { 3,2,0 };
}
