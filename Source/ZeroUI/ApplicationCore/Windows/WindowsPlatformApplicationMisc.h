#pragma once
#include "Core.h"

#include "ApplicationCore/GenericPlatform/GenericPlatformApplicationMisc.h"

namespace ZeroUI
{
	struct FWindowsPlatformApplicationMisc : public FGenericPlatformApplicationMisc
	{
		static void PreInit();
		static void PumpMessages(bool bFromMainLoop);
		static float GetDPIScaleFactorAtPoint(float X, float Y);
		static class GenericApplication* CreateApplication();
	};

	typedef FWindowsPlatformApplicationMisc FPlatformApplicationMisc;

}