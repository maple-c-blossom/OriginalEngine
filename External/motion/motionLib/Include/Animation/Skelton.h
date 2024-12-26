#pragma once
#include <memory>
#include <unordered_map>

#include <Animation/Bone.h>
#include <Animation/Capture.h>


namespace MCBM
{
	class Skelton
	{
	private:
		std::vector<std::unique_ptr<Bone>> bones_;
		
		Bone* rootBone_;

		Capture* capture;

	public:
		void SetCapturePtr(Capture* cap);

		void AddBone(std::unique_ptr<Bone> bone);

		Bone* GetBone(std::string name);

		Quaternion GetBoneRotation(std::string name);

		//キャプチャーデータの読み込み
		void UpdateCaptureData();

		//モデルの初期姿勢の設定（プレイヤーにモデルの素のポーズをまねてもらう）
		void CaptureBasePoseInitialize();

		void SetRootBone(Bone* bone);

		void CaptureBoneAccept();

		void Finalize();
		/// <summary>
		/// モデルの回転の計算
		/// </summary>
		/// <param name="rootBoneName">キャプチャさせたい根本のKey(例:YOLO_POSE_INDEX::SHOULDER_L)</param>
		/// <param name="boneCount">どのくらい先端のボーンまでやるか。腕なら2で良い</param>
		void CaptureBoneUpdate(YOLO_POSE_INDEX rootBoneName, uint32_t boneCount = 2);
	};
}
