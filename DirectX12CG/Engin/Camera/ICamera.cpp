#include "ICamera.h"

void MCB::ICamera::MatrixUpdate()
{
	projection.UpdataMatrixProjection();
	view.UpDateMatrixView();
}
