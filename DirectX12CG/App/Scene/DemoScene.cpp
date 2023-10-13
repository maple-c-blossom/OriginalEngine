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
    for (auto& obj : effectorObjects_)
    {
        obj.Update();
    }
    test2Animation_.animationModel_->skeleton.JointObjectMatrixUpdate(viewCamera_,
        &test2Animation_,boxModel_.get());
}

void MCB::DemoScene::Update()
{
	
	if ( debugView_ )test2Animation_.color_.w_ = { 0.25f };
	else test2Animation_.color_.w_ = { 1.0f };
    if (objChenge_)
    {
        test2Animation_.animationModel_ = animModel_.get();
    }
    else
    {
        test2Animation_.animationModel_ = anim2Model_.get();
    }
    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_RETURN) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        soundManager_->PlaySoundWave(selectSound_);
        sceneEnd_ = true;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        if (noMove[i]) continue;
        DirectX::XMFLOAT3* pos = &effectorObjects_[i].position_;

        if (PoleVecMove_[i])
        {
            pos = &poleVec_[i];
        }
        else
        {
            pos = &effectorObjects_[i].position_;
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
    }

    for (uint8_t i = 0; i < 4; i++)
    {

        if (isIk_[i])
        {
            if (objChenge_)
            {
                test2Animation_.animationModel_->skeleton.SetTwoBoneIK(test2Animation_,
                    { effectorObjects_[0].position_.x,effectorObjects_[0].position_.y,effectorObjects_[0].position_.z },
                    { poleVec_[0].x,poleVec_[0].y,poleVec_[0].z }, "Bone3");
            }
            else
            {
				//test2Animation_.animationModel_->skeleton.GetNode(ikBoneName_[ i ].endJointName)->ikData.isCollisionIk = true;
                test2Animation_.animationModel_->skeleton.SetTwoBoneIK(test2Animation_,
                    { effectorObjects_[i].position_.x,effectorObjects_[i].position_.y,effectorObjects_[i].position_.z },
                    { poleVec_[i].x,poleVec_[i].y,poleVec_[i].z }, ikBoneName_[i].endJointName.c_str(),ikBoneName_[i].middleJointName.c_str(),ikBoneName_[i].rootJointName.c_str());
            }
        }
        else if(!objChenge_)
        {
            test2Animation_.animationModel_->skeleton.TwoBoneIKOff(ikBoneName_[i].endJointName.c_str());
            test2Animation_.animationModel_->skeleton.TwoBoneIKOff("Bone3");
        }
		else if(objChenge_  && isIk_[0])
		{
			test2Animation_.animationModel_->skeleton.TwoBoneIKOff(ikBoneName_[i].endJointName.c_str());
			test2Animation_.animationModel_->skeleton.TwoBoneIKOff("Bone3");
		}
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
	for ( auto& obj : effectorObjects_ )
	{
		obj.Draw();
	}
	test2Animation_.animationModel_->skeleton.JointObjectDraw();
	pipeline_->SetLinePipeLine();
	test2Animation_.animationModel_->skeleton.JointLineDraw();
	if ( !objInvisibleView_ )
	{
	
		pipeline_->SetFbxPipeLine();
		test2Animation_.AnimationDraw();
	}
    pipeline_->SetObjPipeLine();
    postEffect_->PostDraw();
}

void MCB::DemoScene::Draw()
{
    //3Dオブジェクト


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
	if ( ImGui::TreeNode("説明") )
	{
		ImGui::Text("このデモシーンはIKの挙動を確認するためのシーンです。");
		ImGui::Text("球がEffector,四角錐がPoleVectorです");
		ImGui::Text("操作:WASDでEffectorやPoleVectorを移動させることができます");
		ImGui::Text("操作:SPACE,LCONTROLでEffectorやPoleVectorを上下に移動させることができます");
		ImGui::TreePop();
	}

	ImGui::Text(" ");
	ImGui::Text("objPos:%f,%f,%f",test2Animation_.position_.x,test2Animation_.position_.y,test2Animation_.position_.z);
	ImGui::Checkbox("半透明表示",&debugView_);
	ImGui::Checkbox("オブジェクトの表示を表示しない",&objInvisibleView_);

    if (ImGui::TreeNode("IK 制御"))
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            std::string bone = ikBoneName_[i].endJointName;
            if (ImGui::TreeNode(bone.c_str()))
            {
				ImGui::Text("ONの時、IKを行う");
                bone = bone + ":isIK";
                ImGui::Checkbox(bone.c_str(), &isIk_[i]);
				ImGui::Text("ONの時、エフェクター,PoleVectorの移動を禁止");
				bone = ikBoneName_[ i ].endJointName;
                bone = bone + ":エフェクター,PoleVectorの移動を禁止";
                ImGui::Checkbox(bone.c_str(), &noMove[i]);
                bone = ikBoneName_[i].endJointName;
				ImGui::Text("ONの時、エフェクターの代わりにPoleVectorを移動する");
                bone = bone + ":PoleVectorを動かす(Effectorは動かない)";
                ImGui::Checkbox(bone.c_str(), &PoleVecMove_[i]);
                bone = ikBoneName_[i].endJointName;
				ImGui::Text("エフェクターの位置");
                bone = bone + ":effector";
                ImGui::Text("%s:%f,%f,%f", bone.c_str(), effectorObjects_[i].position_.x, effectorObjects_[i].position_.y, effectorObjects_[i].position_.z);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    //ImGui::Checkbox("IkModelChenge", &objChenge_);
    test2Animation_.animationModel_->DrawHeirarchy();

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
	for ( uint8_t i = 0; i < 4; i++ )
	{
		test2Animation_.animationModel_->skeleton.SetTwoBoneIK(test2Animation_,
			{ effectorObjects_[ i ].position_.x,effectorObjects_[ i ].position_.y,effectorObjects_[ i ].position_.z },
			{ poleVec_[ i ].x,poleVec_[ i ].y,poleVec_[ i ].z },ikBoneName_[ i ].endJointName.c_str(),ikBoneName_[ i ].middleJointName.c_str(),ikBoneName_[ i ].rootJointName.c_str());
		test2Animation_.animationModel_->skeleton.TwoBoneIKOff(ikBoneName_[ i ].endJointName.c_str());

	}
}

void MCB::DemoScene::LoadModel()
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
    play_.position_ = { 0,2,0 };
    play_.camera_ = viewCamera_;


    Skydorm_.Init();
    Skydorm_.model_ = skydomeModel_.get();
    Skydorm_.scale_ = { 4,4,4 };
    Skydorm_.camera_ = viewCamera_;

    for (uint8_t i = 0; i < 4; i++)
    {
        effectorObjects_[i].Init();
        //testsphere.model = BoxModel;
        effectorObjects_[i].model_ = sphereModel_.get();
        effectorObjects_[i].scale_ = { 0.05f,0.05f,0.05f };
        effectorObjects_[i].position_ = effectorPos[i];
        effectorObjects_[i].rotation_.y = ConvertRadius(90);
        effectorObjects_[i].camera_ = viewCamera_;
    }
    test2Animation_.animationModel_ = anim2Model_.get();
    test2Animation_.scale_ = { 0.01f,0.01f,0.01f };
    test2Animation_.position_ = { 0,2,0 };
    test2Animation_.camera_ = viewCamera_;

}

MCB::DemoScene::IKDataSet::IKDataSet()
{

};

MCB::DemoScene::IKDataSet::IKDataSet(std::string endJoint)
{
	endJointName = endJoint;
}
MCB::DemoScene::IKDataSet::IKDataSet(std::string endJoint,std::string middlejoint)
{
	endJointName = endJoint;
	middleJointName = middlejoint;
}
MCB::DemoScene::IKDataSet::IKDataSet(std::string endJoint,std::string middleJoint,std::string rootJoint)
{
	endJointName = endJoint;
	middleJointName = middleJoint;
	rootJointName = rootJoint;
};