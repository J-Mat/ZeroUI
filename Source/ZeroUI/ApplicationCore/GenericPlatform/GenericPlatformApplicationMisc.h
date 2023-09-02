#pragma once

namespace ZeroUI
{
	struct FGenericPlatformApplicationMisc
	{
		static void PreInit();

		static void Init();
		static void platform_pre_init(){}
		/**
		* Returns monitor's DPI scale factor at given screen coordinates (expressed in pixels)
		* @return Monitor's DPI scale factor at given point
		*/
		static float GetDPIScaleFactorAtPoint(float X, float Y)
		{
			return 1.0f;
		}
	};
}