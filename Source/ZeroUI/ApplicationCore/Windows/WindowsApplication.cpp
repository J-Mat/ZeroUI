#include "WindowsApplication.h"
#include "WindowsCursor.h"

namespace ZeroUI
{
	// Current instance
	FWindowsApplication* g_WindowsApplication = nullptr;

	FWindowsApplication* FWindowsApplication::CreateWindowsApplication(const HINSTANCE InstanceHandle, const HICON IconHandle)
	{
		g_WindowsApplication = new FWindowsApplication(InstanceHandle, IconHandle);
		return g_WindowsApplication;
	}

	FWindowsApplication::~FWindowsApplication()
	{

	}

	void FWindowsApplication::PumpMessages(const float TimeDelta)
	{
		MSG Message;

		// standard Windows message handling
		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	FWindowsApplication::FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle)
		:GenericApplication(CreateRef<FWindowsCursor>())
		,m_InstanceHandle(HInstance)
	{
		const bool bClassRegistered = RegisterClass(m_InstanceHandle, IconHandle);
	}

	bool FWindowsApplication::RegisterClass(const HINSTANCE HInstance, const HICON HIcon)
	{
		WNDCLASS wc{};
		wc.style = CS_DBLCLKS; // We want to receive double clicks
		wc.lpfnWndProc = AppWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = HInstance;
		wc.hIcon = HIcon;
		wc.hCursor = NULL;	// We manage the cursor ourselves
		wc.hbrBackground = NULL; // Transparent
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"ZeroUI";

		if (!::RegisterClass(&wc))
		{
			MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		return true;
	}

}

