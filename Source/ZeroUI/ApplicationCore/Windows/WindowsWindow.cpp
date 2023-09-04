#include "WindowsWindow.h"
#include "ApplicationCore/GenericPlatform/GenericWindowDefinition.h"

namespace ZeroUI
{
	FWindowsWindow::FWindowsWindow()
		: m_HWnd(nullptr)
		, m_WindowMode(EWindowMode::Windowed)
		, m_AspectRatio(1.0f)
		, m_DPIScaleFactor(1.0f)
	{
	}

	FWindowsWindow::~FWindowsWindow()
	{
		// NOTE: The HWnd is invalid here!
		//       Doing stuff with HWnds will fail silently.
		//       Use Destroy() instead.
	}

	Ref<FWindowsWindow> FWindowsWindow::Make()
	{
		return Ref<FWindowsWindow>();
	}

	HWND FWindowsWindow::GetHWnd() const
	{
		return m_HWnd;
	}

	void FWindowsWindow::Initialize(FWindowsApplication* const Application, const Ref<FGenericWindowDefinition>& InDefinition, HINSTANCE InHInstance, const Ref<FWindowsWindow>& InParent, const bool bShowImmediately)
	{
		m_Definition = InDefinition;
		m_OwningApplication = Application;
		
		// Finally, let's initialize the new native window object.  Calling this function will often cause OS
		// window messages to be sent! (such as activation messages)
		uint32_t WindowExStyle = 0;
		uint32_t WindowStyle = 0;

		m_RegionWidth = m_RegionHeight = INDEX_NONE;

		const float XInitialRect = m_Definition->XDesiredPositionOnScreen;
		const float YInitialRect = m_Definition->YDesiredPositionOnScreen;


		const float WidthInitial = m_Definition->WidthDesiredOnScreen;
		const float HeightInitial = m_Definition->HeightDesiredOnScreen;

		m_DPIScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(XInitialRect, YInitialRect);
	}
}

