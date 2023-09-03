#include "DemoScene.h"
#include "TitleScene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::DemoScene::SpriteInit()
{

    debugText_.Init(debugTextTexture_->texture.get());
    titleSprite_.CreateSprite();
}

void MCB::DemoScene::ParticleInit()
{
}

unique_ptr<MCB::IScene> MCB::DemoScene::GetNextScene()
{
	return move(make_unique<TitleScene>(rootparamsPtr_, depth_, pipeline_));
}

void MCB::DemoScene::MatrixUpdate()
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

void MCB::DemoScene::Update()
{

    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }
    DirectX::XMFLOAT3* pos = &testsphere_.position_;

    if (PoleVecMove)
    {
        pos = &poleVec;
    }
    else
    {
        pos = &testsphere_.position_;
    }


    if (input_->IsKeyDown(DIK_W))
    {
        pos->z += 0.05f;
    }
    
    if (input_->IsKeyDown(DIK_S))
    {
        pos->z -= 0.05f;
    }

    if (input_->IsKeyDown(DIK_D))
    {
        pos->x += 0.05f;
    }

    if (input_->IsKeyDown(DIK_A))
    {
        pos->x -= 0.05f;
    }

    if (input_->IsKeyDown(DIK_SPACE))
    {
        pos->y += 0.05f;
    }

    if (input_->IsKeyDown(DIK_LCONTROL))
    {
        pos->y -= 0.05f;
    }

    if (isIk)
    {
        test2Animation_.animationModel_->skeleton.SetTwoBoneIK({ test2Animation_.position_.x,test2Animation_.position_.y,test2Animation_.position_.z },
            { testsphere_.position_.x,testsphere_.position_.y,testsphere_.position_.z },
            {poleVec.x,poleVec.y,poleVec.z}, "Wrist.L");
    }
    else
    {
        test2Animation_.animationModel_->skeleton.TwoBoneIKOff("Wrist.L");
    }
    MatrixUpdate();
}

void MCB::DemoScene::PostEffectDraw()
{
    postEffect_->PreDraw();
    //pipeline_->SetObjPipeLine(false, true);
    Skydorm_.Draw();
    pipeline_->SetObjPipeLine();
    ground_.Draw();
    testsphere_.Draw();
    if (debugView)
    {
        test2Animation_.animationModel_->skeleton.JointObjectDraw();
        pipeline_->SetLinePipeLine();
        test2Animation_.animationModel_->skeleton.JointLineDraw();
    }
    pipeline_->SetFbxPipeLine();
    if (debugView)
    {
        pipeline_->SetFbxPipeLine(true);
    }
    else test2Animation_.AnimationDraw();
    pipeline_->SetObjPipeLine();
    postEffect_->PostDraw();
}

void MCB::DemoScene::Draw()
{
    //3D�I�u�W�F�N�g


}

void MCB::DemoScene::SpriteDraw()
{
    
    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();
    //titleSprite_.SpriteDraw(*titleTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_);
    debugText_.Print(dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_ + 200, 2, "Press AButton");
    debugText_.AllDraw();
}

void MCB::DemoScene::ParticleDraw()
{
}

void MCB::DemoScene::CheckAllColision()
{
}

void MCB::DemoScene::ImGuiUpdate()
{
    imgui_.Begin();
    ImGui::Checkbox("debugView", &debugView);
    ImGui::Checkbox("isIK", &isIk);
    ImGui::Checkbox("poleVectorMove", &PoleVecMove);
    test2Animation_.animationModel_->DrawHeirarchy();
    ImGui::Text("effector:%f,%f,%f", testsphere_.position_.x, testsphere_.position_.y, testsphere_.position_.z);
    ImGui::Text("testAni:%f,%f,%f", test2Animation_.position_.x, test2Animation_.position_.y, test2Animation_.position_.z);
    imgui_.End();
}

MCB::DemoScene::DemoScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline)
{
	rootparamsPtr_ = root;
	depth_ = depth;
    pipeline_ = pipeline;
}

MCB::DemoScene::~DemoScene()
{
    //soundManager_->AllDeleteSound();
    debugTextTexture_->free = true;

    //modelManager_->erase();
    loader_->Erase();
}

void MCB::DemoScene::Initialize()
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

void MCB::DemoScene::LoadModel()
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

void MCB::DemoScene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    titleTex_ = loader_->LoadTexture(L"Resources\\Title.png");
}

void MCB::DemoScene::LoadSound()
{
    selectSound_ = soundManager_->LoadWaveSound("Resources\\sounds\\select.wav");
    soundManager_->SetVolume(100, selectSound_);
}

void MCB::DemoScene::Object3DInit()
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