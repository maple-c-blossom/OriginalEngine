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

		if ( isMove )
		{
			updated = true;
			totalMoveVec = moveVec.vec_ * speed;
			position_.x += totalMoveVec.vec_.x_;
			position_.y += totalMoveVec.vec_.y_;
			position_.z += totalMoveVec.vec_.z_;
			moveLength += totalMoveVec.V3Len();
			if ( maxMoveLength <= abs(moveLength) )
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
