#include "GenericWindow.h"


namespace ZeroUI
{
	FGenericWindow::FGenericWindow()
	{

	}

	FGenericWindow::~FGenericWindow()
	{

	}

	void* FGenericWindow::GetOSWindowHandle() const
	{
		return nullptr;
	}

	float FGenericWindow::GetDPIScaleFactor() const
	{
		return 1.0f;
	}

	void FGenericWindow::SetDPIScaleFactor(const float Factor)
	{
		// empty default functionality
	}
}