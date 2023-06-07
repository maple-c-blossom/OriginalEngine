#include "TitleScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{

    debugText.Init(debugTextTexture->texture.get());
}

void MCB::TitleScene::ParticleInit()
{
}

shared_ptr<MCB::IScene> MCB::TitleScene::GetNextScene()
{
	return make_shared<Scene>(rootparamsPtr, depth, pipeline);
}

void MCB::TitleScene::MatrixUpdate()
{
    //play.UpdateMatrix();
    camera.Update();
    Skydorm.Update(viewCamera);
    ground.Update(viewCamera);
    testsphere.AnimationUpdate(viewCamera);
    test2Animation.AnimationUpdate(viewCamera);
}

void MCB::TitleScene::Update()
{

    lights->UpDate();
    if (input->IsKeyTrigger(DIK_RETURN) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
    {
        sceneEnd = true;
    }
    play.Update();
    MatrixUpdate();
}

void MCB::TitleScene::PostEffectDraw()
{
    postEffect->PreDraw();
    Skydorm.Draw();
    ground.Draw();
    play.Draw();
    pipeline->SetFbxPipeLine();
    testsphere.AnimationDraw();
    test2Animation.AnimationDraw();
    pipeline->SetObjPipeLine();
    postEffect->PostDraw();
}

void MCB::TitleScene::Draw()
{
    //3Dオブジェクト


}

void MCB::TitleScene::SpriteDraw()
{
    postEffect->Draw();
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
            ImGui::Text("SceneChange: [ENTER] or [GamePad A]");
            ImGui::Text("sphereMove: [WASD]");
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
    debugTextTexture->free = true;
    loader->Erase();
}

void MCB::TitleScene::Initialize()
{
    camera.Inilialize();
    viewCamera = &camera;
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
    postEffect->Init();
}

void MCB::TitleScene::LoadModel()
{

    groundModel = std::make_unique<Model>("ground");

    skydomeModel = std::make_unique<Model>("skydome");

    sphereModel = std::make_unique<Model>("sphere");


    animModel = std::make_unique<AnimationModel>();
    animModel->Load("gamewsdsa");

    anim2Model = std::make_unique<AnimationModel>();
    anim2Model->Load("testFbx");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
}

void MCB::TitleScene::LoadSound()
{
    testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::TitleScene::Object3DInit()
{

    ground.Init();
    ground.model = groundModel.get();
    ground.scale = { 1,1,1 };
    ground.position = { 0,-3,0 };
    ground.rotasion = { 0,0,ConvertRadius(5) };
    ground.SetCollider(std::move(std::make_shared<MeshCollider>(groundModel.get())));
    ground.camera = viewCamera;

    play.Init();
    play.model = sphereModel.get();
    play.position = { 0,3,0 };
    play.camera = viewCamera;

    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel.get();
    Skydorm.scale = { 4,4,4 };
    Skydorm.camera = viewCamera;

    testsphere.Init();
    //testsphere.model = BoxModel;
    testsphere.animationModel = animModel.get();
    testsphere.scale = { 3,3,3 };
    testsphere.position = { 0,4,30 };
    testsphere.rotasion.y = ConvertRadius(90);
    testsphere.camera = viewCamera;

    test2Animation.animationModel = anim2Model.get();
    test2Animation.scale = { 3,3,3 };
    test2Animation.position = { 10,4,30 };
    test2Animation.rotasion.y = ConvertRadius(90);
    test2Animation.camera = viewCamera;
}
