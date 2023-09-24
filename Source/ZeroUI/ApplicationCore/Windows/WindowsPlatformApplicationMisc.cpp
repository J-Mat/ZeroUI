#include "WindowsPlatformApplicationMisc.h"
#include "WindowsApplication.h"

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

	class GenericApplication* FWindowsPlatformApplicationMisc::CreateApplication(HINSTANCE hInst)
	{
		HICON AppIconHandle = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
		return FWindowsApplication::CreateWindowsApplication(hInst, AppIconHandle);
	}

}
