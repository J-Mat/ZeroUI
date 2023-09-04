#include "WindowsPlatformApplicationMisc.h"

namespace ZeroUI
{
	void FWindowsPlatformApplicationMisc::PreInit()
	{
	}

	void FWindowsPlatformApplicationMisc::PumpMessages(bool bFromMainLoop)
	{
	}

	float FWindowsPlatformApplicationMisc::GetDPIScaleFactorAtPoint(float X, float Y)
	{
		return 1.0f;
	}

	GenericApplication* FWindowsPlatformApplicationMisc::CreateApplication()
	{

	}
}
