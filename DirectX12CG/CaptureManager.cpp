//#include "CaptureManager.h"
//
//void MCB::CaptureManager::Initialize()
//{
//	for ( int i = 0; i < 4; i++ )
//	{
//		switch ( static_cast< Locate >( i ) )
//		{
//		case Locate::FRONT:
//			capdatas[ i ].SetSideCaptureFlag(false);
//			break;
//		case Locate::BACK:
//			capdatas[ i ].SetSideCaptureFlag(false);
//			break;
//		case Locate::RIGHT:
//			capdatas[ i ].SetSideCaptureFlag(true);
//			break;
//		case Locate::LEFT:
//			capdatas[ i ].SetSideCaptureFlag(true);
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//void MCB::CaptureManager::Update()
//{
//	for ( int i = 0; i < 4; i++ )
//	{
//		capdatas[ i ].Update();
//	}
//}
//
//void MCB::CaptureManager::CalclateFinalCaptureData()
//{
//	Vector3D finalData;
//	for ( int32_t i = 0; i < ( int32_t ) YOLO_POSE_INDEX::YOLO_POSE_INDEX_MAX; i++ )
//	{
//		for ( int i = 0; i < 4; i++ )
//		{
//
//		}
//	}
//}
