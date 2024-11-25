#pragma once

enum class ONNXP_ROVIDERS
{
	CPU,
	CUDA,
	DIRECTML
};

enum class YOLO_POSE_INDEX
{
	NOSE,
	EYE_L,
	EYE_R,
	EAR_L,
	EARR,
	SHOULDER_L,
	SHOULDER_R,
	ELBOW_L,
	ELBOW_R,
	WRIST_L,
	WRIST_R,
	HIP_L,
	HIP_R,
	KNEE_L,
	KNEE_R,
	ANKLE_L,
	ANKLE_R,

	YOLO_POSE_INDEX_MAX
};

struct YOLO_POSE_LANDMAKE
{
	float x;
	float y;
	float vi;
};

class YOLOPoseEstimation
{
public:

	YOLOPoseEstimation() = default;

	virtual ~YOLOPoseEstimation() = default;

	virtual void CameraInitialize(void* cam) = 0;

	virtual void ModelInitialize(const char* modelPath, float mask_threshold = 0.5f, float conf_threshold = 0.30f, float iou_threshold = 0.45f, ONNXP_ROVIDERS provider = ONNXP_ROVIDERS::DIRECTML) = 0;

	virtual void Start(bool isDraw= false) = 0;

	virtual void End() = 0;

	virtual const YOLO_POSE_LANDMAKE* const GetLandmakes() = 0;

};


YOLOPoseEstimation* CreateYOLOPoseEstimation();
