#include "RotationBlock.h"

void MCB::RotationBlock::UniqueUpdate()
{
	
	rotation_.x += rotateSpeed_.vec_.x_;
	rotation_.y += rotateSpeed_.vec_.y_;
	rotation_.z += rotateSpeed_.vec_.z_;
}
