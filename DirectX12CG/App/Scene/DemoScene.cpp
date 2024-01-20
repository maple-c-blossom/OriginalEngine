#include "DemoScene.h"
#include "TitleScene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::DemoScene::SpriteInit()
{
	sprite_.CreateSprite();
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


	for ( auto& obj : poleVecObjects_ )
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
    lights_->UpDate();
    if (input_->IsKeyTrigger(DIK_SPACE) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A))
    {
        //soundManager_->PlaySoundWave(selectSound_);
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
		if ( collIK[ i ] )
		{
			test2Animation_.animationModel_->skeleton.SetCollTwoIK(ikBoneName_[ i ].endJointName.c_str(),true);
			test2Animation_.animationModel_->skeleton.SetConstraint(test2Animation_,ikBoneName_[ i ].endJointName.c_str(),poleVec_[i]);

		}
        else if (isIk_[i] )
        {
			test2Animation_.animationModel_->skeleton.SetCollTwoIK(ikBoneName_[ i ].endJointName.c_str(),false);
			
			test2Animation_.animationModel_->skeleton.SetTwoBoneIK(test2Animation_,
                    { effectorObjects_[i].position_.x,effectorObjects_[i].position_.y,effectorObjects_[i].position_.z },
                    { poleVec_[i].x,poleVec_[i].y,poleVec_[i].z }, ikBoneName_[i].endJointName.c_str(),ikBoneName_[i].middleJointName.c_str(),ikBoneName_[i].rootJointName.c_str());
        }
        else if(!collIK[i] )
        {
			test2Animation_.animationModel_->skeleton.SetCollTwoIK(ikBoneName_[ i ].endJointName.c_str(),false);
            test2Animation_.animationModel_->skeleton.TwoBoneIKOff(ikBoneName_[i].endJointName.c_str());

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
	pipeline_->SetObjPipeLine(false,false);
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
	sprite_.SpriteDraw(*enter->texture.get(),dxWindow_->sWINDOW_CENTER_WIDTH_,dxWindow_->sWINDOW_HEIGHT_ - 100);
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
	if ( ImGui::Button("ギズモの表示") )
	{
		gizmoDraw_ = !gizmoDraw_;
	}


	size_t matId = 0;
	if ( gizmoDraw_ )
	{
		for ( auto& obj : effectorObjects_ )
		{
			ImGuizmo::SetID(static_cast< int32_t >( matId ));
			matId++;
			ImguiManager::GuizmoDraw(&obj,ImGuizmo::OPERATION::TRANSLATE,ImGuizmo::LOCAL);
		}


		for ( auto& poleObj : poleVecObjects_ )
		{
			ImGuizmo::SetID(static_cast< int32_t >( matId ));
			poleVec_[ matId - 4u ] = poleObj.position_;
			matId++;
			ImguiManager::GuizmoDraw(&poleObj,ImGuizmo::OPERATION::TRANSLATE,ImGuizmo::LOCAL);
		}

	}
	ImGuizmo::SetID(static_cast< int32_t >( matId ));
	ImguiManager::GuizmoDraw(&ground_,ImGuizmo::OPERATION::TRANSLATE,ImGuizmo::LOCAL);

	if ( ImGui::TreeNode("説明") )
	{
		ImGui::Text("このデモシーンはIKの挙動を確認するためのシーンです。");
		ImGui::Text("球がEffector,四角錐がPoleVectorです");
		ImGui::TreePop();
	}

	ImGui::Text(" ");
	ImGui::Text("objPos:%f,%f,%f",test2Animation_.position_.x,test2Animation_.position_.y,test2Animation_.position_.z);
	ImGui::Checkbox("半透明表示",&debugView_);
	ImGui::Checkbox("オブジェクトを表示しない",&objInvisibleView_);
    if (ImGui::TreeNode("IK 制御"))
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            std::string bone = ikBoneName_[i].endJointName;
            if (ImGui::TreeNode(bone.c_str()))
            {
				ImGui::Checkbox("当たり判定に基づいたIK",&collIK[i]);
				if ( collIK[i])
				{
					ImGui::Text("現在、当たり判定に基づいてIKを行っています。");
					ImGui::Text("手動で確認をしたい場合、");
					ImGui::Text("「当たり判定に基づいたIK」のチェックを外してください");
				}
				else
				{
					ImGui::Text("ONの時、IKを行う");
					ImGui::Checkbox("isIK", &isIk_[i]);
				}
				ImGui::Text("EffectorとPoleVectorまでの線を描画");
				ImGui::Checkbox("LineDraw",&test2Animation_.animationModel_->skeleton.GetNode(bone)->lineView);
				ImGui::Text("三角形を描画");
				ImGui::Checkbox("TriangleDraw",&test2Animation_.animationModel_->skeleton.GetNode(bone)->ikData.triangleDraw);
                bone = ikBoneName_[i].endJointName;
				ImGui::Text("エフェクターの位置");
                bone = bone + ":effector";
                ImGui::Text("%s:%f,%f,%f", bone.c_str(), effectorObjects_[i].position_.x, effectorObjects_[i].position_.y, effectorObjects_[i].position_.z);
                ImGui::TreePop();
            }
        }
		if ( ImGui::TreeNode("IKData") )
		{
			test2Animation_.animationModel_->skeleton.DrawIkNode();
			ImGui::TreePop();
		}
        ImGui::TreePop();
    }

	if ( ImGui::TreeNode("アニメーション関連") )
	{
		ImGui::InputFloat("AnimationSpeed",&animationSpeed);
		if ( ImGui::Button("アニメーション再生") )
		{
			animePlay = !animePlay;
			test2Animation_.animeTime_ = 0.f;
		}

		if ( animePlay )
		{
			test2Animation_.animationSpeed_ = animationSpeed;
		}
		else
		{
			test2Animation_.animationSpeed_ = 0;
		}

		if ( ImGui::BeginCombo("Animation",animationName[ animationNum ].c_str()) )
		{
			for ( uint8_t i = 0; i < animationName.size(); i++ )
			{
				if ( ImGui::Selectable(animationName[ i ].c_str(),i == animationNum) )
				{
					animationNum = i;

				}
			}
			ImGui::EndCombo();
		}


		test2Animation_.currentAnimation_ = animationName[ animationNum ];
		float animTime = test2Animation_.animeTime_;
		ImGui::SliderFloat("AnimTime",&animTime,0.f,7.f);
		test2Animation_.animeTime_ = animTime;
		ImGui::TreePop();
	}

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
		//test2Animation_.animationModel_->skeleton.TwoBoneIKOff(ikBoneName_[ i ].endJointName.c_str());

	}
		test2Animation_.currentAnimation_ = "Tpose..";
		LightGroup::GetInstance()->SetDirLightIsActive(0,true);
		LightGroup::GetInstance()->SetSLightIsActive(0,true);
		LightGroup::GetInstance()->SetSLightIsActive(1,true);

		LightGroup::GetInstance()->SetSLightForLightDir(0,{0,0,1});
		LightGroup::GetInstance()->SetSLightForLightDir(1,{0,0,-1});
		LightGroup::GetInstance()->SetSLightPos(0,
			{ test2Animation_.position_.x,test2Animation_.position_.y,test2Animation_.position_.z - 1.f });
		LightGroup::GetInstance()->SetSLightPos(1,
			{ test2Animation_.position_.x,test2Animation_.position_.y,test2Animation_.position_.z + 1.f });

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
	enter = loader_->LoadTexture(L"Resources\\PressEnter.png");
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
    ground_.position_ = { 0,2,0 };
    ground_.rotation_ = { 0,0,0 };
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
        effectorObjects_[i].camera_ = viewCamera_;


		poleVecObjects_[ i ].Init();
	//testsphere.model = BoxModel;
		poleVecObjects_[ i ].model_ = sphereModel_.get();
		poleVecObjects_[ i ].scale_ = { 0.05f,0.05f,0.05f };
		poleVecObjects_[ i ].position_ = poleVec_[ i ];
		poleVecObjects_[ i ].camera_ = viewCamera_;
    }
    test2Animation_.animationModel_ = anim2Model_.get();
    test2Animation_.scale_ = { 0.01f,0.01f,0.01f };
    test2Animation_.position_ = { 0,2,0 };
    test2Animation_.camera_ = viewCamera_;
	test2Animation_.currentAnimation_ = "Tpose";
	test2Animation_.AnimationUpdate();
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