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

MCB::IScene* MCB::TitleScene::GetNextScene()
{
	return new Scene(rootparamsPtr, depth, pipeline);
}

void MCB::TitleScene::MatrixUpdate()
{
 
    Skydorm.Update(viewCamera);
    ground.Update(viewCamera);
    testAnimation.AnimationUpdate(viewCamera);
    test2Animation.AnimationUpdate(viewCamera);
}

void MCB::TitleScene::Update()
{

    lights->UpDate();
    if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
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
    test2Animation.AnimationDraw();
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
            ImGui::Text("SceneChange: [SPACE] or [GamePad A]");
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
    delete nextScene;
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
   
}

void MCB::TitleScene::LoadModel()
{

    groundModel = std::make_unique<Model>("ground");

    skydomeModel = std::make_unique<Model>("skydome");


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


    ground;
    ground.Init();
    ground.model = groundModel.get();
    ground.scale = { 4,4,4 };
    ground.position = { 0,0,0 };
    ;
    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel.get();
    Skydorm.scale = { 4,4,4 };

    testAnimation.Init();
    //testAnimation.model = BoxModel;
    testAnimation.animationModel = animModel.get();
    testAnimation.scale = { 3,3,3 };
    testAnimation.position = { 0,4,10 };
    testAnimation.rotasion.y = ConvertRadius(90);
    
    test2Animation.animationModel = anim2Model.get();
    test2Animation.scale = { 3,3,3 };
    test2Animation.position = { 10,4,10 };
    test2Animation.rotasion.y = ConvertRadius(90);
}
