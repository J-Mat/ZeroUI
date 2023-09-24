#pragma once
#include "Core.h"
#include "SlateCore/Rendering/SlateLayoutTransform.h"

namespace ZeroUI
{
	class FLayoutGeometry
	{
	public:
		FLayoutGeometry()
			: m_LocalSize(1.0f, 1.0f)
		{
		}

		explicit FLayoutGeometry(const FSlateLayoutTransform& InLocalToParent, const ZMath::vec2& SizeInLocalSpace)
		: m_LocalToParent(InLocalToParent)
		, m_LocalSize(SizeInLocalSpace)
		{
		}
		const FSlateLayoutTransform& GetLocalToParentTransform() const
		{
			return m_LocalToParent;
		}

		ZMath::vec2 get_size_in_local_space() const
		{
			return m_LocalSize;
		}
	private:
		FSlateLayoutTransform m_LocalToParent;
		ZMath::vec2 m_LocalSize;
	};
}