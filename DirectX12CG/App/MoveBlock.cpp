#include "MoveBlock.h"

void MCB::MoveBlock::UniqueUpdate()
{
	if ( updated )
	{
		updated = false;
	}

	if ( !updated )
	{
		if ( !isMove )
		{
			defaultPos = position_;
		}
		if ( !isSetMove )
		{
			isMove = true;
		}
		totalMoveVec = { 0,0,0 };
		if ( isMove )
		{
			updated = true;
			if ( maxMoveLength > abs(moveLength) )
			{
				totalMoveVec = moveVec.vec_ * speed;
				position_.x += totalMoveVec.vec_.x_;
				position_.y += totalMoveVec.vec_.y_;
				position_.z += totalMoveVec.vec_.z_;
				moveLength += totalMoveVec.V3Len();
				rockTime.Set(100);
			}
			else if ( !rockTime.IsEnd() )
			{
				
				rockTime.Update();
			}
			else if ( rockTime.IsEnd() )
			{
				moveLength = 0;
				moveVec *= -1;
			}
		}

		if ( defaultPos == position_ )
		{
			isMove = false;
		}
	}
}
