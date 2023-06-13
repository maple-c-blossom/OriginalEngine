#include "ICamera.h"

void MCB::ICamera::MatrixUpdate()
{
	projection_.UpdataMatrixProjection();
	view_.UpDateMatrixView();
}
