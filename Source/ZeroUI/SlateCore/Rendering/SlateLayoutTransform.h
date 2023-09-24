#pragma once

#include "Core/Math/TransformCalculus.h"
#include "Core/Math/TransformCalculus2D.h"

namespace ZeroUI
{
	/**
	 * Represents a 2D transformation in the following order: scale then translate.
	 * Used by FGeometry for it's layout transformations.
	 *
	 * Matrix form looks like:
	 *   [Vx Vy 1] * [ S   0   0 ]
	 *               [ 0   S   0 ]
	 *               [ Tx  Ty  1 ]
	 *
	 */
	class FSlateLayoutTransform
	{
	public:
		FSlateLayoutTransform(float InScale = 1.0f)
		: m_Scale(InScale)
		, m_Translation(ZMath::vec2(0.0f, 0.0f))
		{
		}

		FSlateLayoutTransform(const ZMath::vec2& InTranslation)
			: m_Scale(1.0f)
			, m_Translation(InTranslation)
		{
		}

		/** Ctor from a scale followed by translate. Shortcut to Concatenate(InScale, InTranslation). */
		explicit FSlateLayoutTransform(float InScale = 1.0f, const ZMath::vec2& InTranslation = ZMath::vec2(0.0f, 0.0f))
			: m_Scale(InScale)
			, m_Translation(InTranslation)
		{
		}

		/*access to the 2d translation*/
		ZMath::vec2 GetTranslation() const
		{
			return m_Translation;
		}

		/*access to the scale*/
		float GetScale() const
		{
			return m_Scale;
		}

		/*2d transform support*/
		ZMath::vec2 TransformPoint(const ZMath::vec2& Point) const
		{
			return ZeroUI::TransformPoint(GetTranslation(), ZeroUI::TransformPoint(m_Scale, Point));
		}

		/*2d transform support*/
		ZMath::vec2 TransformVector(const ZMath::vec2& Vector) const
		{
			return ZeroUI::TransformVector(GetTranslation(), ZeroUI::TransformVector(m_Scale, Vector));
		}

		FSlateLayoutTransform Inverse() const
		{
			return FSlateLayoutTransform(ZeroUI::Inverse(m_Scale), ZeroUI::Inverse(m_Translation) * ZeroUI::Inverse(m_Scale));
		}

		/**
		 * This works by transforming the origin through LHS then RHS.
		 * In matrix form, looks like this:
		 * [ Sa  0   0 ]   [ Sb  0   0 ]
		 * [ 0   Sa  0 ] * [ 0   Sb  0 ]
		 * [ Tax Tay 1 ]   [ Tbx Tby 1 ]
		 */
		FSlateLayoutTransform Concatenate(const FSlateLayoutTransform& RHS) const
		{
			// New Translation is essentially: RHS.TransformPoint(TransformPoint(FVector2D::ZeroVector))
			// Since Zero through LHS -> Translation we optimize it slightly to skip the zero multiplies.
			return FSlateLayoutTransform(ZeroUI::Concatenate(m_Scale, RHS.m_Scale), RHS.TransformPoint(m_Translation));
		}

	private:
		float m_Scale;
		ZMath::vec2 m_Translation;
	};

	template<> template<> inline FTransform2D TransformConverter<FTransform2D>::Convert<FSlateLayoutTransform>(const FSlateLayoutTransform& Transform)
	{
		return FTransform2D(Transform.GetScale(), Transform.GetTranslation());
	}
}