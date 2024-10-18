#pragma once
#include <string>
#include "Vector3D.h"
#include "Quaternion.h"

class IKSolver
{
private:
	std::string effector_;

	MCB::Vector3D targetPos_;

	int32_t iterations_;

	float limitRadian_;

	MCB::Quaternion defRotation;

	int32_t linkBone_;

public:
	void Set();
	void Update();
};

/*
struct IKLink
{
	~IKLink() {
		if ( axis )
		{
			delete axis;
		}
	}
	std::int32_t linkBone;
	Value_ptr<Bone> vlp_linkBone;
	bool radianLimit = false;
	float bonesLength = 0;
	Quat befBoneInitRotation;
	Vector3 upLimit;
	Vector3 downLimit;
	Vector3* axis = nullptr;

	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(radianLimit);
		ARCHIVE_BUTI(upLimit);
		ARCHIVE_BUTI(downLimit);
		ARCHIVE_BUTI(bonesLength);
		ARCHIVE_BUTI(linkBone);
	}
};
struct IKData
{
	BUTIRENDERING_API ~IKData();
	std::int32_t targetBoneIndex;
	std::int32_t extremityBoneIndex;
	Value_ptr<Bone> vlp_targetBone;
	std::int32_t loopCount;
	float maxRadian;
	float sum_bonesLength = 0;
	std::vector<IKLink> links;
	//Vector3* p_jointPoints=nullptr;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(targetBoneIndex);
		ARCHIVE_BUTI(extremityBoneIndex);
		ARCHIVE_BUTI(loopCount);
		ARCHIVE_BUTI(maxRadian);
		ARCHIVE_BUTI(sum_bonesLength);
		ARCHIVE_BUTI(links);
	}
};
*/