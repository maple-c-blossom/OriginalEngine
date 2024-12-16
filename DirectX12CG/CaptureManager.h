#pragma once
#include "Capture.h"
#include <array>
#include <memory>

namespace MCB
{
	enum class Locate : int32_t
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};
	class CaptureManager
	{

	private:
		std::array<Capture,4> capdatas;
		std::unordered_map<YOLO_POSE_INDEX,CaptureData> finalCaptureData_;
	public:
		void Initialize();
		void Update();
		void CalclateFinalCaptureData();


	};
}
