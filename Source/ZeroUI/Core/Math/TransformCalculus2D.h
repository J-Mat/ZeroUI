#pragma once

#include "ZMath.h"
#include "TransformCalculus.h"

namespace ZeroUI
{
	class FScale2D
	{
	public:
		/* ctor, initialize to an identity scale, 1.0 */
		FScale2D() : m_Scale(1.0f, 1.0f){}

		/* ctor, initialize from a uniform scale */
		explicit FScale2D(float InScale) : m_Scale(InScale, InScale) {}

		/* access to the underlying FVector2D that stores the scale */
		const ZMath::vec2& GetVector() const { return m_Scale; }
	private:
		/*underlying storage of the 2D scale*/
		ZMath::vec2 m_Scale;
	};

	/*
	 * 2x2 generalized matrix
	 * as FMatrix, we assume row vectors, row major storage
	 * [X Y] * [m00 m01]
	 *		   [m10 m11]
	 */
	class FMatrix2x2
	{
	public:
		/*ctor initialize to an identity*/
		FMatrix2x2()
		{
			m_M[0][0] = 1; m_M[0][1] = 0;
			m_M[1][0] = 0; m_M[1][1] = 1;
		}

		explicit FMatrix2x2(const FScale2D& Scale)
		{
			float ScaleX = Scale.GetVector().X;
			float ScaleY = Scale.GetVector().Y;

			m_M[0][0] = ScaleX; m_M[0][1] = 0;
			m_M[1][0] = 0;      m_M[1][1] = ScaleY;
		}

		FMatrix2x2(float m00, float m01, float m10, float m11)
		{
			m_M[0][0] = m00, m_M[0][1] = m01;
			m_M[1][0] = m10, m_M[1][1] = m11;
		}

		void GetMatrix(float& A, float& B, float& C, float& D) const
		{
			A = m_M[0][0];
			B = m_M[0][1];
			C = m_M[1][0];
			D = m_M[1][1];
		}

		/*
		 * transform a 2d point
		 * [X Y] * [m00 m01]
		 *		   [m10 m11]
		 */
		ZMath::vec2 TransformPoint(const ZMath::vec2& Point) const
		{
			return ZMath::vec2(Point.x * m_M[0][0] + Point.y * m_M[1][0],
				Point.y * m_M[0][1] + Point.y * m_M[1][1]);
		}

		/*vector transformation is equivalent to point transformation as our matrix is not homogeneous*/
		ZMath::vec2 TransformVector(const ZMath::vec2& Vector) const
		{
			return TransformPoint(Vector);
		}

		/*
		 * concatenate 2 matrices:
		 * [A B] * [E F] == [AE+BG AF+BH]
		 * [C D] * [G H] == [CE+DG CF+DH]
		 */
		FMatrix2x2 Concatenate(const FMatrix2x2& RHS) const
		{
			float a, b, c, d;
			GetMatrix(a, b, c, d);
			float e, f, g, h;
			RHS.GetMatrix(e, f, g, h);
			return FMatrix2x2(
				a * e + b * g, a * f + b * h,
				c * e + d * g, c * f + d * h
			);
		};

	private:
		float m_M[2][2];
	};

	/*
	 * support for generalized 2d affine transforms
	 * implemented as a 2x2 transform followed by translation. in matrix form:
	 * [A B 0]
	 * [C D 0]
	 * [X Y 1]
	 */
	class FTransform2D
	{
	public:
		/* initialize the transform using an identity matrix and a translation */
		FTransform2D(const ZMath::vec2& Translation = ZMath::vec2(0.0f, 0.0f))
			: m_Trans(Translation)
		{
		}

		/* initialize the transform using a uniform scale an a translation */
		explicit FTransform2D(float uniform_scale, const ZMath::vec2& translation = ZMath::vec2(0.0f, 0.0f))
			: m_M(FScale2D(uniform_scale)), m_Trans(translation)
		{
			//FScale2D scale_2d(uniform_scale);
		}

		explicit FTransform2D(const FMatrix2x2& transform, const ZMath::vec2& translation = ZMath::vec2(0.0f, 0.0f))
			: m_M(transform), m_Trans(translation)
		{
		}

		/*
		 * 2d transformation of a vector, transforms rotation and scale
		 */
		ZMath::vec2 TransformVector(const ZMath::vec2& Vector) const
		{
			return ZeroUI::TransformVector(m_M, Vector);
		}

		const FMatrix2x2& GetMatrix() const { return m_M; }

		/*access to the translation*/
		const ZMath::vec2 GetTranslation() const { return m_Trans; }

		void set_translation(const ZMath::vec2& in_trans) { m_Trans = in_trans; }

		/*
		 * concatenates two transforms, result is equivalent to transforming first by this, followed by rhs
		 * concat(A, B) == (P * MA + TA) * MB + TB
		 *				== (P * MA * MB) + TA * MB + TB
		 *
		 *	NewM == MA * MB
		 *	NewT == TA * MB + TB
		 */
		FTransform2D Concatenate(const FTransform2D& RHS) const
		{
			//ZeroUI::transform_point(rhs.m_m, m_trans);
			return FTransform2D(
				ZeroUI::Concatenate(m_M, RHS.m_M),
				ZeroUI::TransformPoint(RHS.m_M, m_Trans) + RHS.m_Trans)
			;
		}

		/*
		 * 2d transformation of a point, transforms position, rotation, and scale
		 */
		ZMath::vec2 TransformPoint(const ZMath::vec2& Point) const
		{
			return ZeroUI::TransformPoint(GetTranslation(), ZeroUI::TransformPoint(m_M, Point));
		}

	private:
		FMatrix2x2 m_M;//matrix
		ZMath::vec2 m_Trans;
	};

	//inline FTransform2D concatenate(const ZMath::vec2)
	inline ZMath::vec2 Concatenate(const ZMath::vec2& LHS, const ZMath::vec2& RHS)
	{
		return LHS + RHS;
	}

	/*specialization for concatenating 2d translation and transform, see docs for FTransform2D::inverse for details on how this math is derived*/
	inline FTransform2D Concatenate(const ZMath::vec2& Translation, const FTransform2D& Transform)
	{
		//ZeroUI::concatenate(ZeroUI::transform_point(transform.get_matrix(), translation), transform.get_translation());
		return FTransform2D(Transform.GetMatrix(), Concatenate(ZeroUI::TransformPoint(Transform.GetMatrix(), Translation), Transform.GetTranslation())); 
	}

	template<typename TransformType> 
	struct ConcatenateRules<TransformType, typename std::enable_if<!std::is_same<FTransform2D, TransformType>::value, FTransform2D>::type>
	{ 
		typedef FTransform2D ResultType; 
	};
	template<typename TransformType> 
	struct ConcatenateRules<typename std::enable_if<!std::is_same<FTransform2D, TransformType>::value, FTransform2D>::type, TransformType> 
	{ 
		typedef FTransform2D ResultType; 
	};
	template<> struct ConcatenateRules <FMatrix2x2, FTransform2D> { typedef FTransform2D ResultType; };
	template<> struct ConcatenateRules <FTransform2D, FMatrix2x2> { typedef FTransform2D ResultType; };
}