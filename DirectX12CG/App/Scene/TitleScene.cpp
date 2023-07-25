#include "TitleScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{

    debugText_.Init(debugTextTexture_->texture.get());
    titleSprite_.CreateSprite();
}

void MCB::TitleScene::ParticleInit()
{
}

unique_ptr<MCB::IScene> MCB::TitleScene::GetNextScene()
{
	return move(make_unique<Scene>(rootparamsPtr_, depth_, pipeline_));
}

void MCB::TitleScene::MatrixUpdate()
{
    //test2Animation_.UpdateMatrix();
    camera_.Update();
    Skydorm_.Update(viewCamera_);
    ground_.Update(viewCamera_);
    testsphere_.Update(viewCamera_);
    test2Animation_.AnimationUpdate(viewCamera_);
}

void MCB::TitleScene::Update()
{

    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_SPACE) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }

    if (input_->IsKeyDown(DIK_W))
    {
        testsphere_.position_.z += 0.01f;
    }
    
    if (input_->IsKeyDown(DIK_S))
    {
        testsphere_.position_.z -= 0.01f;
    }

    if (input_->IsKeyDown(DIK_D))
    {
        testsphere_.position_.x += 0.01f;
    }

    if (input_->IsKeyDown(DIK_A))
    {
        testsphere_.position_.x -= 0.01f;
    }


    test2Animation_.animationModel_->skeleton.SetTwoBoneIK({ test2Animation_.position_.x,test2Animation_.position_.y,test2Animation_.position_.z },
        { testsphere_.position_.x,testsphere_.position_.y,testsphere_.position_.z});
    MatrixUpdate();
}

void MCB::TitleScene::PostEffectDraw()
{
    postEffect_->PreDraw();
    Skydorm_.Draw();
    ground_.Draw();
    testsphere_.Draw();
    pipeline_->SetFbxPipeLine();
    test2Animation_.AnimationDraw();

    pipeline_->SetObjPipeLine();
    postEffect_->PostDraw();
}

void MCB::TitleScene::Draw()
{
    //3Dオブジェクト


}

void MCB::TitleScene::SpriteDraw()
{
    
    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();
    titleSprite_.SpriteDraw(*titleTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_);
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
    soundManager_->AllDeleteSound();
    debugTextTexture_->free = true;

    //modelManager_->erase();
    loader_->Erase();
}

void MCB::TitleScene::Initialize()
{
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

void MCB::TitleScene::LoadModel()
{

    groundModel_ = std::make_unique<Model>("ground");

    skydomeModel_ = std::make_unique<Model>("skydome");

    sphereModel_ = std::make_unique<Model>("sphere");


    animModel_ = std::make_unique<AnimationModel>();
    animModel_->Load("gamewsdsa");

    anim2Model_ = std::make_unique<AnimationModel>();
    anim2Model_->Load("IKTest");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    titleTex_ = loader_->LoadTexture(L"Resources\\Title.png");
}

void MCB::TitleScene::LoadSound()
{
    selectSound_ = soundManager_->LoadWaveSound("Resources\\select.wav");
    test2Sound_ = soundManager_->LoadWaveSound("Resources\\fanfare.wav");
    soundManager_->SetVolume(100, selectSound_);
}

void MCB::TitleScene::Object3DInit()
{

    ground_.Init();
    ground_.model_ = groundModel_.get();
    ground_.scale_ = { 1,1,1 };
    ground_.position_ = { 0,-3,0 };
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
    testsphere_.scale_ = { 1,1,1 };
    testsphere_.position_ = { 10,4,25 };
    testsphere_.rotation_.y = ConvertRadius(90);
    testsphere_.camera_ = viewCamera_;

    test2Animation_.animationModel_ = anim2Model_.get();
    test2Animation_.scale_ = { 3,3,3 };
    test2Animation_.position_ = { 10,4,30 };
    test2Animation_.camera_ = viewCamera_;
}
