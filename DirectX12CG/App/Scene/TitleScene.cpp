#include "TitleScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{

    debugText_.Init(debugTextTexture_->texture.get());
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
    //play.UpdateMatrix();
    camera_.Update();
    Skydorm_.Update(viewCamera_);
    ground_.Update(viewCamera_);
    testsphere_.AnimationUpdate(viewCamera_);
    test2Animation_.AnimationUpdate(viewCamera_);
}

void MCB::TitleScene::Update()
{

    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        sceneEnd_ = true;
    }
    play_.Update();
    MatrixUpdate();
}

void MCB::TitleScene::PostEffectDraw()
{
    postEffect_->PreDraw();
    Skydorm_.Draw();
    ground_.Draw();
    play_.Draw();
    pipeline_->SetFbxPipeLine();
    testsphere_.AnimationDraw();
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
    XMMATRIX mat = testsphere_.animationModel_->bones_[0].finalMatrix;
    //ImGui::ShowDemoWindow();
    if (ImGui::CollapsingHeader("Infomation"))
    {
        if (ImGui::TreeNode("operation"))
        {
            ImGui::Text("SceneChange: [ENTER] or [GamePad A]");
            ImGui::Text("sphereMove: [WASD]");
            ImGui::Text("finalMat[0]...   %f,%f,%f,%f"
                , mat.r[0].m128_f32[0], mat.r[0].m128_f32[1], mat.r[0].m128_f32[2], mat.r[0].m128_f32[1] );
            ImGui::Text("finalMat[0]...   %f,%f,%f,%f"
                , mat.r[1].m128_f32[0], mat.r[1].m128_f32[1], mat.r[1].m128_f32[2], mat.r[1].m128_f32[1]);
            ImGui::Text("finalMat[0]...   %f,%f,%f,%f"
                , mat.r[2].m128_f32[0], mat.r[2].m128_f32[1], mat.r[2].m128_f32[2], mat.r[2].m128_f32[1]);
            ImGui::Text("finalMat[0]...   %f,%f,%f,%f"
                , mat.r[3].m128_f32[0], mat.r[3].m128_f32[1], mat.r[3].m128_f32[2], mat.r[3].m128_f32[1]);
            ImGui::TreePop();
        }
    }
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
    anim2Model_->Load("testFbx");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
}

void MCB::TitleScene::LoadSound()
{
    testSound_ = soundManager_->LoadWaveSound("Resources\\cat1.wav");
    test2Sound_ = soundManager_->LoadWaveSound("Resources\\fanfare.wav");
    soundManager_->SetVolume(100, testSound_);
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
    testsphere_.animationModel_ = animModel_.get();
    testsphere_.scale_ = { 3,3,3 };
    testsphere_.position_ = { 0,4,30 };
    testsphere_.rotation_.y = ConvertRadius(90);
    testsphere_.camera_ = viewCamera_;

    test2Animation_.animationModel_ = anim2Model_.get();
    test2Animation_.scale_ = { 3,3,3 };
    test2Animation_.position_ = { 10,4,30 };
    test2Animation_.rotation_.y = ConvertRadius(90);
    test2Animation_.camera_ = viewCamera_;
}
