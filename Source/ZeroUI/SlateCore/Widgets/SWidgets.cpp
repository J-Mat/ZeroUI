#include "SWidgets.h"

namespace ZeroUI
{
	SWidget::~SWidget()
	{
	}

	void SWidget::AssignParentWidget(Ref<SWidget> InParent)
	{
		m_ParentWidgetPtr = InParent;
	}

	ZMath::vec2 SWidget::GetDesiredSize() const
	{
		return ZMath::vec2(m_DesiredSize.value_or(ZMath::vec2(0.0f)));
	}

	void SWidget::CacheDesiredSize(float InLayoutScaleMultiplier)
	{
		SetDesiredSize(ComputeDesiredSize(InLayoutScaleMultiplier));
	}

}

