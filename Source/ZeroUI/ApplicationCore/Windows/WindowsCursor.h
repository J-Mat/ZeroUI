#pragma once
#include "Core.h"
#include "ApplicationCore/GenericPlatform/ICursor.h"

namespace ZeroUI
{
	class FWindowsCursor : public ICursor
	{
	public:

		FWindowsCursor();

		virtual ~FWindowsCursor();

		virtual ZMath::vec2 GetPosition() const override;

		virtual void SetPosition(const int32_t X, const int32_t Y) override;



	private:

		EMouseCursor::Type CurrentType;

		/** Cursors */
		HCURSOR CursorHandles[EMouseCursor::TotalCursorCount];

		/** Override Cursors */
		HCURSOR CursorOverrideHandles[EMouseCursor::TotalCursorCount];
	};
}



