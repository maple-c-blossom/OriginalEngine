#include "Capture.h"

void MCB::Capture::Initialize()
{
	cv::VideoCapture cap(0);
	capture_ = cap;
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
}

void MCB::Capture::Update()
{
	cv::imshow("test", img_);

	land_ = m_YOLOPoseEstimation_->GetLandmakes();

	//skelton構成


}

void MCB::Capture::Finalize()
{
	m_YOLOPoseEstimation_->End();
	cv::destroyWindow("test");
}
