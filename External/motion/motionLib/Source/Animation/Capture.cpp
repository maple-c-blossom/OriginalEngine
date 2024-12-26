#include "Capture.h"
#include "Util.h"
void MCB::Capture::Initialize()
{
	cv::VideoCapture cap(0);
	capture_ = std::move(cap);
	capture_.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	capture_.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	capture_.set(cv::CAP_PROP_FPS, 30);
	capture_.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('H', '2', '6', '4'));



	if (!capture_.isOpened())
	{
		initialized_ = false;
		return;
	}

	img_ = cv::imread("Resources\\Rairu.jpg");

	m_YOLOPoseEstimation_.reset(CreateYOLOPoseEstimation());

	m_YOLOPoseEstimation_->CameraInitialize(&capture_);

	m_YOLOPoseEstimation_->ModelInitialize(modelPath_.c_str());

	m_YOLOPoseEstimation_->Start(true);
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		CaptureData datatemp;
		capturedata_[(YOLO_POSE_INDEX)i] = datatemp;
	}
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		CaptureData* temp = &capturedata_[(YOLO_POSE_INDEX)i];
		temp->captureBoneName = linkBoneNames[i];
		switch ((YOLO_POSE_INDEX)i)
		{
		case YOLO_POSE_INDEX::NOSE:

			break;
		case YOLO_POSE_INDEX::EYE_L:

			break;
		case YOLO_POSE_INDEX::EYE_R:

			break;
		case YOLO_POSE_INDEX::EAR_L:

			break;
		case YOLO_POSE_INDEX::EARR:

			break;
		case YOLO_POSE_INDEX::SHOULDER_L:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::ELBOW_L]);
			break;
		case YOLO_POSE_INDEX::SHOULDER_R:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::ELBOW_R]);
			break;
		case YOLO_POSE_INDEX::ELBOW_L:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::WRIST_L]);
			break;
		case YOLO_POSE_INDEX::ELBOW_R:
			temp->captureChildren.push_back(&capturedata_[YOLO_POSE_INDEX::WRIST_R]);
			break;
		case YOLO_POSE_INDEX::WRIST_L:

			break;
		case YOLO_POSE_INDEX::WRIST_R:

			break;
		case YOLO_POSE_INDEX::HIP_L:

			break;
		case YOLO_POSE_INDEX::HIP_R:

			break;
		case YOLO_POSE_INDEX::KNEE_L:

			break;
		case YOLO_POSE_INDEX::KNEE_R:

			break;
		case YOLO_POSE_INDEX::ANKLE_L:

			break;
		case YOLO_POSE_INDEX::ANKLE_R:

			break;
		default:
			break;
		}
	}

}

void MCB::Capture::Update()
{
	cv::imshow("run", img_);

	land_ = m_YOLOPoseEstimation_->GetLandmakes();

	//skeltonç\ê¨
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		capturedata_[(YOLO_POSE_INDEX)i].captureBonePos = { Util::TruncateToTens(land_[i].x),Util::TruncateToTens(land_[i].y),0 };
	}

}

void MCB::Capture::SetInitialPose()
{
	land_ = m_YOLOPoseEstimation_->GetLandmakes();

	//skeltonç\ê¨
	for (int32_t i = 0; i < (int32_t)YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++)
	{
		capturedata_[(YOLO_POSE_INDEX)i].initializedCaptureBonePos = { Util::TruncateToTens(land_[i].x),Util::TruncateToTens(land_[i].y),0 };
		capturedata_[(YOLO_POSE_INDEX)i].captureBonePos = { Util::TruncateToTens(land_[i].x),
			Util::TruncateToTens(land_[i].y),0 };
	}
}

void MCB::Capture::Finalize()
{
	m_YOLOPoseEstimation_->End();
	cv::destroyWindow("run");
}

MCB::CaptureData& MCB::Capture::GetCaptureData(YOLO_POSE_INDEX key)
{
	return capturedata_[key];
}
