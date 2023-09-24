#pragma once

#include "Core.h"

namespace ZeroUI
{
	/*
	 * a rectangle defined by upper-Left and lower-Left corners
	 *
	 * assumes a "screen-like" coordinate system where the origin is in the Top-Left, with the y-axis going down
	 * functions like "contains" etc will not work with other conventions
	 */
	class FSlateRect
	{
	public:
		float Left;
		float Top;
		float Right;
		float Bottom;

		explicit FSlateRect(float InLeft = -1, float InTop = - 1, float InRight = -1, float InBottom = -1)
			: Left(InLeft)
			, Top(InTop)
			, Right(InRight)
			, Bottom(InBottom)
		{}

		FSlateRect(const ZMath::vec2& InStartPos, const ZMath::vec2& InEndPos)
			: Left(InStartPos.x)
			, Top(InStartPos.y)
			, Right(InEndPos.x)
			, Bottom(InEndPos.y)
		{}

		/*
		 * returns the Top-Left position of the rectangle
		 * @return the Top-Left position
		 */
		ZMath::vec2 get_Top_Left() const
		{
			return ZMath::vec2(Left, Top);
		}

		ZMath::vec2 get_Bottom_Right() const
		{
			return ZMath::vec2(Right, Bottom);
		}

	private:

	};
}