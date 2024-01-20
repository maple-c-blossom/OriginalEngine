#include "MiniatureGardenScene.h"
#include "TitleScene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::MiniatureGardenScene::SpriteInit()
{
	sprite_.CreateSprite();
}

void MCB::MiniatureGardenScene::ParticleInit()
{
}

unique_ptr<MCB::IScene> MCB::MiniatureGardenScene::GetNextScene()
{
	return move(make_unique<TitleScene>(rootparamsPtr_,depth_,pipeline_));
}

void MCB::MiniatureGardenScene::MatrixUpdate()
{
	//test2Animation_.UpdateMatrix();
	viewCamera_->Update();
	Skydorm_.Update();
	ground_.Update();
	play_.AnimationUpdate();
	for ( auto& obj : effectorObjects_ )
	{
		obj.Update();
	}


	for ( auto& obj : poleVecObjects_ )
	{
		obj.Update();
	}
}

void MCB::MiniatureGardenScene::Update()
{

	play_.UniqueUpdate();
	

	MatrixUpdate();
}

void MCB::MiniatureGardenScene::PostEffectDraw()
{
	postEffect_->PreDraw();
	//pipeline_->SetObjPipeLine(false, true);
	Skydorm_.Draw();
	pipeline_->SetObjPipeLine();
	ground_.Draw();
	for ( auto& obj : effectorObjects_ )
	{
		obj.Draw();
	}
	pipeline_->SetObjPipeLine(false,false);
	play_.animationModel_->skeleton.JointObjectDraw();
	pipeline_->SetLinePipeLine();
	//play_.animationModel_->skeleton.JointLineDraw();
	pipeline_->SetFbxPipeLine();
	play_.AnimationDraw();
	postEffect_->PostDraw();
}

void MCB::MiniatureGardenScene::Draw()
{
	//3Dオブジェクト


}

void MCB::MiniatureGardenScene::SpriteDraw()
{

	postEffect_->Draw();
	pipeline_->SetSpritePipeLine();
	//titleSprite_.SpriteDraw(*titleTex_->texture.get(), dxWindow_->sWINDOW_CENTER_WIDTH_, dxWindow_->sWINDOW_CENTER_HEIGHT_);
	sprite_.SpriteDraw(*enter->texture.get(),dxWindow_->sWINDOW_CENTER_WIDTH_,dxWindow_->sWINDOW_HEIGHT_ - 100);
}

void MCB::MiniatureGardenScene::ParticleDraw()
{
}

void MCB::MiniatureGardenScene::CheckAllColision()
{
}

void MCB::MiniatureGardenScene::ImGuiUpdate()
{
	imgui_.Begin();
	

	
	imgui_.End();
}

MCB::MiniatureGardenScene::MiniatureGardenScene(RootParameter* root,Depth* depth,PipeLineManager* pipeline)
{
	rootparamsPtr_ = root;
	depth_ = depth;
	pipeline_ = pipeline;
}


MCB::MiniatureGardenScene::~MiniatureGardenScene()
{
	//soundManager_->AllDeleteSound();
	debugTextTexture_->free = true;

	//modelManager_->erase();
	loader_->Erase();
}

void MCB::MiniatureGardenScene::Initialize()
{
	camera_.Inilialize();
	camera_.SetCameraTarget(&play_);
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

void MCB::MiniatureGardenScene::LoadModel()
{

	groundModel_ = std::make_unique<Model>("ground");

	skydomeModel_ = std::make_unique<Model>("skydome");

	sphereModel_ = std::make_unique<Model>("sphere");

	boxModel_ = std::make_unique<Model>("Bone");


	animModel_ = std::make_unique<AnimationModel>();
	animModel_->Load("IKTest");

	anim2Model_ = std::make_unique<AnimationModel>();
	anim2Model_->Load("player");
}

void MCB::MiniatureGardenScene::LoadTexture()
{
	debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
	enter = loader_->LoadTexture(L"Resources\\PressEnter.png");
}

void MCB::MiniatureGardenScene::LoadSound()
{
	selectSound_ = soundManager_->LoadWaveSound("Resources\\sounds\\select.wav");
	soundManager_->SetVolume(100,selectSound_);
}

void MCB::MiniatureGardenScene::Object3DInit()
{

	ground_.Init();
	ground_.model_ = groundModel_.get();
	ground_.scale_ = { 1,1,1 };
	ground_.position_ = { 0,0,0 };
	ground_.rotation_ = { 0,0,0 };
	ground_.SetCollider(std::move(std::make_unique<MeshCollider>(groundModel_.get())));
	ground_.camera_ = viewCamera_;


	Skydorm_.Init();
	Skydorm_.model_ = skydomeModel_.get();
	Skydorm_.scale_ = { 4,4,4 };
	Skydorm_.camera_ = viewCamera_;
	play_.animationModel_ = anim2Model_.get();
	play_.Init();
	play_.position_ = { 0,0.5f,0 };
}

MCB::MiniatureGardenScene::IKDataSet::IKDataSet()
{

};

MCB::MiniatureGardenScene::IKDataSet::IKDataSet(std::string endJoint)
{
	endJointName = endJoint;
}
MCB::MiniatureGardenScene::IKDataSet::IKDataSet(std::string endJoint,std::string middlejoint)
{
	endJointName = endJoint;
	middleJointName = middlejoint;
}
MCB::MiniatureGardenScene::IKDataSet::IKDataSet(std::string endJoint,std::string middleJoint,std::string rootJoint)
{
	endJointName = endJoint;
	middleJointName = middleJoint;
	rootJointName = rootJoint;
};