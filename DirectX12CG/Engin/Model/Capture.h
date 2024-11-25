#pragma once

#include <opencv2/opencv.hpp>
#include <YOLOPoseEstimation.h>
#include <xmemory>
#include <string>
#include <vector>
#include "Vector3D.h"

namespace MCB
{
	struct CaptureData
	{
		std::string captureBoneName;
		Vector3D captureBonePos;
		Vector3D initializedCaptureBonePos;
		std::vector<CaptureData*> captureChildren;
		CaptureData* parent;

	};

	class Capture
	{
	private:
		cv::VideoCapture capture_;
		cv::Mat img_;
		const std::string& modelPath_ = "Resources/Checkpoints/yolo11x-pose.onnx";
		std::unique_ptr<YOLOPoseEstimation> m_YOLOPoseEstimation_;
		const YOLO_POSE_LANDMAKE* land_;
		float mask_threshold_ = 0.5f;
		float conf_threshold_ = 0.30f;
		float iou_threshold_ = 0.45f;
		int conversion_code_ = cv::COLOR_BGR2RGB;
		bool initialized_ = false;

		void Initialize();
		void Update();
		void Finalize();
	};

}