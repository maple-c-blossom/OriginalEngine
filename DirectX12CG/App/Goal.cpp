#include "Goal.h"
#include "DxWindow.h"
#include "DebugText.h"
using namespace MCB;
using namespace std;

bool Goal::goal_ = false;
int32_t Goal::timer_ = SECOND_FROM_MINITE * 3;
void Goal::ResetGoal()
{
	goal_ = false;
	timer_ = SECOND_FROM_MINITE * 3;
}
void Goal::Init()
{
	Object3d::Init();
	goal_ = false;
	timer_ = SECOND_FROM_MINITE * 3;
	numSprite[0].CreateSprite();
	numSprite[1].CreateSprite();
	numSprite[2].CreateSprite();
	timeSprite.CreateSprite();
	goalSprite.CreateSprite();
	pressSprite.CreateSprite();
	RankSprite_.CreateSprite();
	RankNumTexs[ 0 ] = TextureManager::GetInstance()->LoadTexture(L"Resources\\S.png");
	RankNumTexs[ 1 ] = TextureManager::GetInstance()->LoadTexture(L"Resources\\A.png");
	RankNumTexs[ 2 ] = TextureManager::GetInstance()->LoadTexture(L"Resources\\B.png");
	RankNumTexs[ 3 ] = TextureManager::GetInstance()->LoadTexture(L"Resources\\C.png");
	RankTex = TextureManager::GetInstance()->LoadTexture(L"Resources\\Rank.png");
	for ( size_t i = 0; i < 4; i++ )
	{
		RankNumSprites[ i ].CreateSprite();
	}
	numTex_ = TextureManager::GetInstance()->LoadTexture(L"Resources\\num.png");
	timeTex_ = TextureManager::GetInstance()->LoadTexture(L"Resources\\time.png");
	goalTex_ = TextureManager::GetInstance()->LoadTexture(L"Resources\\Goal.png");
	pressTex_ = TextureManager::GetInstance()->LoadTexture(L"Resources\\PressEnter.png");
	SetCollider(make_unique<SphereCollider>(Vector3D{0,0,0},2.f));
	collider_->SetAttribute(ATTRIBUTE_ENEMY);
	UpdateMatrix();
	collider_->Update();
}

void Goal::UniqueUpdate()
{
	if (player->position_.z >= position_.z)
	{
		goal_ = true;
	}
	if (!goal_)
	{
		timer_--;
		timer_ = max(0,timer_);
	}
	for (auto& effect : effects_)
	{
		effect->UniqueUpdate();
		effect->UpdateMatrix(camera_);
	}
	effects_.remove_if([](auto& effect) {return effect->deleteFlag_; });
	
}

void Goal::Draw()
{
	if(!goal_)Object3d::Draw();
	for (auto& effect : effects_)
	{
		effect->Draw();
	}
}

void Goal::Draw(uint16_t incremant)
{
	static_cast< void >( incremant );
	if ( !goal_ )Object3d::Draw();
	for ( auto& effect : effects_ )
	{
		effect->Draw();
	}
}

void Goal::DebugTextDraw(MCB::DebugText* debugText)
{

	if (Goal::goal_)
	{
		DxWindow* dxw = DxWindow::GetInstance();
		goalSprite.SpriteDraw(*goalTex_->texture.get(),dxw->sWINDOW_CENTER_WIDTH_,256,
			580 * 2,64 * 2);
		RankSprite_.SpriteDraw(*RankTex->texture.get(),dxw->sWINDOW_CENTER_WIDTH_,256 + 135,
			580 * 2,64 * 2);
		if ( timer_ >= SRank )
		{
			RankNumSprites[0].SpriteDraw(*RankNumTexs[ 0 ]->texture.get(),dxw->sWINDOW_CENTER_WIDTH_ + 64,256 + 135,
			580 * 2,64 * 2);
		}
		else if ( timer_ >= ARank )
		{
			RankNumSprites[ 1 ].SpriteDraw(*RankNumTexs[ 1 ]->texture.get(),dxw->sWINDOW_CENTER_WIDTH_ + 64,256 + 135,
			580 * 2,64 * 2);
		}
		else if ( timer_ >= BRank )
		{
			RankNumSprites[ 2 ].SpriteDraw(*RankNumTexs[ 2 ]->texture.get(),dxw->sWINDOW_CENTER_WIDTH_ + 64,256 + 135,
			580 * 2,64 * 2);
		}
		else
		{
			RankNumSprites[ 3 ].SpriteDraw(*RankNumTexs[ 3 ]->texture.get(),dxw->sWINDOW_CENTER_WIDTH_ + 64,256 + 135,
			580 * 2,64 * 2);
		}
		pressSprite.SpriteDraw(*pressTex_->texture.get(),dxw->sWINDOW_CENTER_WIDTH_,dxw->sWINDOW_CENTER_HEIGHT_ + 250,
			580 * 2,64 * 2);
	}

	int digits = 0;
	int tempTime = timer_ / 60;
	int tempTime2 = tempTime;
	do
	{
		tempTime /= 10;
		digits++;
	} while ( tempTime != 0 );

	int digit = 1;
	for ( int i = 0; i < digits; i++ )
	{
		if ( i == 0 )
		{
			digit = 1;
		}
		else
		{
			digit *= 10;
		}
	}
	vector<int> eachnum;
	
	for ( int i = 0; i < digits; i++ )
	{
		int temp = 0;
		if ( digit != 0 )
		{
			temp = tempTime2 / digit;
			tempTime2 %= digit;
			digit /= 10;
		}
		eachnum.push_back(temp);
	}
	// 描画処理
	int i = digits;
	while ( i > 0 )
	{
		i--;
		numSprite[i].SpriteCuttingDraw(*numTex_->texture.get(),static_cast< float >( DxWindow::GetInstance()->sWINDOW_WIDTH_ ) - 150.f
			+ 35.f * i,42.f,{ 32.f,64.f },{ 32.f * static_cast<float>(eachnum[i]),0.f },32,64);
	}
	timeSprite.SpriteDraw(*timeTex_->texture.get(),static_cast< float >( DxWindow::GetInstance()->sWINDOW_WIDTH_ ) - 250.f,42.f);
}

int32_t Goal::GetTime()
{
	return timer_;
}

bool Goal::GetIsGoal()
{
	if ( goal_ || timer_ <= 0 )
	{
		return true;
	}
	return false;
}

void Goal::OnCollision(const MCB::CollisionInfomation& info)
{
	static_cast< void >( info );
}

void Goal::SetPlayer(MCB::Object3d* playerPtr)
{
	player = playerPtr;
}
