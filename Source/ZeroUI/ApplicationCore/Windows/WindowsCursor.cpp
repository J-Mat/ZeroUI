#include "WindowsCursor.h"

namespace ZeroUI
{ 
	FWindowsCursor::FWindowsCursor()
	{

	}

	FWindowsCursor::~FWindowsCursor()
	{

	}

	ZMath::vec2 FWindowsCursor::GetPosition() const
	{
		POINT CursorPos;
		::GetCursorPos(&CursorPos);

		return ZMath::vec2(CursorPos.x, CursorPos.y);
	}

	void FWindowsCursor::SetPosition(const int32_t X, const int32_t Y)
	{
		::SetCursorPos(X, Y);
	}
}

