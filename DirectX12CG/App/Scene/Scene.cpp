#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

MCB::Scene::~Scene()
{
    //soundManager_.AllDeleteSound();
    //modelManager_->erase();
    loader_->Erase();
}


void MCB::Scene::Initialize()
{

    maincamera_.Inilialize();
    debugCamera_.Inilialize();
    viewCamera_ = &startcamera;
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
    postEffect_->color_.x_ = static_cast<float>(PostEffectNum::NONE);
    maincamera_.SetCameraTarget(level_->GetObjectPtr("player"));
    startTimer.TimeSet(240,240);
    goal_ = dynamic_cast<Goal*>(level_->GetObjectPtr("goal"));
    skydome = level_->GetObjectPtr("backGround");
    player_ = dynamic_cast<Player*>(level_->GetObjectPtr("player"));
    player_->runFast = runFast;
    player_->runNormal = runNormal;
	stickMove[ 0 ] = { centerPos.x_,centerPos.y_ + 10 };
	stickMove[ 1 ] = { centerPos.x_ + 10,centerPos.y_ };
	stickMove[ 2 ] = { centerPos.x_,centerPos.y_ - 10 };
	stickMove[ 3 ] = { centerPos.x_ - 10,centerPos.y_ };
	nextStickPos = stickMove[0];
	prevStickPos = stickPos;
	stickMoveTime.Set(60);
	startcamera.Inilialize();
	startcamera.targetstart_ = goal_;
	startcamera.targetend_ = player_;
}

void MCB::Scene::SetStage(std::string stageName)
{
    stage = stageName;
}

void MCB::Scene::Object3DInit()
{

   
    level_ = move(LevelLoader::Load(stage,viewCamera_));

}


void MCB::Scene::LoadModel()
{

   SpherModel_ = modelManager_->GetModel("sphere", true);
   //groundModel_ = modelManager_->GetModel("ground");
   //skydomeModel_ = modelManager_->GetModel("skydome");
   playerModel_ = modelManager_->GetModel("player", playerModel_);
   goalModel_ = modelManager_->GetModel("star");
    //testModel.Load("Resources\\testFbx\\boneTest.fbx");
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
    debugTextTexture_ = loader_->LoadTexture(L"Resources\\debugfont.png");
    goText = loader_->LoadTexture(L"Resources\\Go.png");
    moveText = loader_->LoadTexture(L"Resources\\Move.png");
    lStickTex = loader_->LoadTexture(L"Resources\\anarogu.png");
	jumpTex = loader_->LoadTexture(L"Resources\\Jump.png");
	numTex = loader_->LoadTexture(L"Resources\\num.png");
	startTex = loader_->LoadTexture(L"Resources\\start.png");

    //zoomTex = loader->LoadTexture(L"Resources\\testenemy.png");
    //zoomTex = loader->CreateNoTextureFileIsTexture();


}

void MCB::Scene::LoadSound()
{
    runNormal = soundManager_->LoadWaveSound("Resources\\sounds\\runNormal.wav");
    runFast = soundManager_->LoadWaveSound("Resources\\sounds\\runFast.wav");
}

void MCB::Scene::SpriteInit()
{
    //postEffect->tex = debugTextTexture;
	sprite_.CreateSprite();
	jumpTexSprite_.CreateSprite();
	lStickTexSprite_.CreateSprite();
	moveTextSprite_.CreateSprite();
	countSprite_.CreateSprite();
	startTexSprite_.CreateSprite();
    debugText_.Init(debugTextTexture_->texture.get());

}

void MCB::Scene::ParticleInit()
{


    //testParticle.rotation.x = ConvertRadius(-90);
}


unique_ptr<IScene> MCB::Scene::GetNextScene()
{
    return move(make_unique<TitleScene>(rootparamsPtr_, depth_, pipeline_));
}



void MCB::Scene::Update()
{
	if ( startcamera.moveTimer.IsEnd() )
	{
		viewCamera_ = &maincamera_;
		startTimer.SafeDownUpdate();
	}
	else
	{
		viewCamera_ = &startcamera;
		
	}
  
    level_->Update(startTimer.NowTime() <= 0 && !goal_->GetIsGoal());
	skydome->position_.z = player_->position_.z;
    if (goal_->GetIsGoal())
    {
        player_->currentAnimation_ = "Idle";
    }
    lights_->UpDate();
    debugCamera_.Update();
    maincamera_.Update();
	stickMoveTime.SafeUpdate();
	if ( stickMoveTime.IsEnd() )
	{
		stickMoveTime.ReSet();
		if ( stickPos.x_ == centerPos.x_ && stickPos.y_ == centerPos.y_ )
		{
			movenum++;
			if ( movenum > 3 )
			{
				movenum = 0;
			}
			prevStickPos = stickPos;
			nextStickPos = stickMove[ movenum ];

		}
		else
		{
			prevStickPos = stickPos;
			nextStickPos = centerPos;
		}
	}

	stickPos.x_ = static_cast< float >(Lerp(static_cast< float >( prevStickPos.x_ ),static_cast< float >( nextStickPos.x_ ),
		stickMoveTime.GetEndTime(),stickMoveTime.NowTime()));
	stickPos.y_ = static_cast< float >(Lerp(static_cast<float>(prevStickPos.y_),static_cast<float>(nextStickPos.y_),
		stickMoveTime.GetEndTime(),stickMoveTime.NowTime()));

	if ( input_->IsKeyTrigger(DIK_N) )
	{
		debugWindow_ = !debugWindow_;
	}


    //goal_.UniqueUpdate();
    //Node* node = playerModel_->ReadNode("LowerArm.R");
    //node->ikData.isIK = true;
    //node->ikData.iKTargetPosition = { player_.position_.x + 1.0f,player_.position_.y + 0.5f,player_.position_.z };
    CheckAllColision();
    MatrixUpdate();
    

    if (goal_->GetIsGoal() && (input_->IsKeyTrigger(DIK_SPACE) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A)))
    {
        sceneEnd_ = true;
    }
}

void MCB::Scene::Draw()
{
  

}

void MCB::Scene::PostEffectDraw()
{
    postEffect_->PreDraw();
    level_->Draw(pipeline_);
    pipeline_->SetFbxPipeLine();
    level_->AnimationDraw();
	//playerModel_->skeleton.GetNode("mixamorig:LeftFoot")->jointView = true;
	pipeline_->SetObjPipeLine(false,false);
	if(debugWindow_ ) player_->animationModel_->skeleton.JointObjectDraw();
	pipeline_->SetLinePipeLine();
	if(debugWindow_) level_->GetObjectPtr("player")->animationModel_->skeleton.JointLineDraw();

    postEffect_->PostDraw();

}

void MCB::Scene::SpriteDraw()
{
    pipeline_->SetPostEffectPipeLine();
    postEffect_->Draw();
    pipeline_->SetSpritePipeLine();

    debugText_.sprite_->color_ = { 1,1,1,1 };

	moveTextSprite_.anchorPoint_ = { 0,0 };
	jumpTexSprite_.anchorPoint_ = { 0,0 };

	moveTextSprite_.SpriteDraw(*moveText->texture.get(),20,dxWindow_->sWINDOW_HEIGHT_ - 130.f,413.f / 1.5f,64.f);
	lStickTexSprite_.SpriteDraw(*lStickTex->texture.get(),stickPos.x_,stickPos.y_);
	jumpTexSprite_.SpriteDraw(*jumpTex->texture.get(),20,dxWindow_->sWINDOW_HEIGHT_ - 60.f,413.f / 1.5f ,64.f);

	//sprite_.SpriteDraw(*goText->texture.get(),dxWindow_->sWINDOW_CENTER_WIDTH_,20);

    if (!(startTimer.NowTime() <= 0) && viewCamera_ == &maincamera_)
    {
        int32_t time = startTimer.NowTime() / 60;
		if ( time > 0 )
		{
			countSprite_.color_ = { 1,1,1,1 };
			countSprite_.SpriteCuttingDraw(*numTex->texture.get(),static_cast< float >( DxWindow::GetInstance()->sWINDOW_CENTER_WIDTH_ )
				,static_cast< float >( DxWindow::GetInstance()->sWINDOW_CENTER_HEIGHT_ - 360.f + 64.f),
				{ 32.f,64.f },{ 32.f * static_cast< float >(time),0.f },32 * 2,64 * 2);
			countSprite_.color_ = { 0,0,0,1 };
		}
        else
        {
            startTexSprite_.SpriteDraw(*startTex->texture.get(),static_cast< float >( DxWindow::GetInstance()->sWINDOW_CENTER_WIDTH_ )
				,static_cast< float >( DxWindow::GetInstance()->sWINDOW_CENTER_HEIGHT_ - 360.f + 64.f ));
            if (!startDrawed)
            {
                startTimer.TimeSet(30, 0);
                startDrawed = true;
            }
        }
    }
    level_->DebugTextDraw(&debugText_);
    //postEffect->Draw();
 /*   sprite.SpriteDraw(*zoomTex->texture.get(), 500, 100);*/

    //debugText.Print(300, 300,2, "hogehoge");
    debugText_.AllDraw();
}

void MCB::Scene::ParticleDraw()
{
    
}

void MCB::Scene::CheckAllColision()
{
    CollisionManager::GetInstance()->CheckAllCollision();
}


void MCB::Scene::ImGuiUpdate()
{
    imgui_.Begin();
    //ImGui::ShowDemoWindow();
    //if (ImGui::CollapsingHeader("Infomation"))
    //{
    //    if (ImGui::TreeNode("operation"))
    //    {
    //        ImGui::Text("LevelReLoad:LCONTROL");
    //        ImGui::TreePop();
    //    }
    //}
	if ( debugWindow_ )
	{
		   if (ImGui::CollapsingHeader("MotionModel"))
		   {
		        playerModel_->DrawHeirarchy();
		   }
		player_->Debug();

	}
    imgui_.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera_->Update();
    level_->UpdateMatrix();
    //player_.AnimationUpdate();
	level_->GetObjectPtr("player")->animationModel_->skeleton.JointObjectMatrixUpdate(viewCamera_,
		level_->GetObjectPtr("player"),goalModel_);
    //goal_.Update();
    //check.Update();

    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipeLineManager* pipeline)
{
    rootparamsPtr_ = root;
    depth_ = depthptr;
    pipeline_ = pipeline;
}
