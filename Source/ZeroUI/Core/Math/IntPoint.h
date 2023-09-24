#pragma once
#include "ZMath.h"

namespace ZeroUI
{
	/*
	 * structure for integer points in 2-d space
	 */
	struct FIntPoint
	{
		union
		{
			struct
			{
				/*holds the point's x-coordinate*/
				int32_t X;

				/*holds the point's y-coordinate*/
				int32_t Y;
			};

			//internal use only
			int32_t XY[2];
		};

	public:
		/*an integer point with zeroed values*/
		static const FIntPoint Zero_Value;

		/*an integer point with INDEX_NONE values*/
		static const FIntPoint None_Value;

	public:
		/*default constructor (no initialization)*/
		FIntPoint();

		/*
		 * create and initialize a new instance with the specified coordinates
		 *
		 * @param InX the x-coordinate
		 * @param InY the y-coordinate
		 */
		FIntPoint(int32_t InX, int32_t InY);

		/*
		 * create and initialize a new instance with a single int
		 * both x and y will be initialized to this value
		 *
		 * @param InXY the x and y-coordinate
		 */
		FIntPoint(int32_t InXY);

	public:
		/*
		 * get specific component of a point
		 *
		 * @param PointIndex Index of point component
		 * @return const reference to component
		 */
		const int32_t& operator()(int32_t point_index) const;
	
		/*
		 * get specific component of a point
		 *
		 * @param PointIndex Index of point component
		 * @return reference to component
		 */
		int32_t& operator()(int32_t point_index);

		/*
		 * compare two points for equality
		 *
		 * @param other the other int point begin compared
		 * @return true if the points are equal, false otherwise
		 */
		bool operator==(const FIntPoint& other) const;

		/*
		 * compare two points for inequality
		 *
		 * @param other the other int point being compared
		 * @return true if the points are not equal, false otherwise
		 */
		bool operator!=(const FIntPoint& other) const;

		/*
		 * scale this point
		 *
		 * @param scale what to multiply the point by
		 * @return reference to this point after multiplication
		 */
		FIntPoint& operator*=(int32_t scale);

		/*
		 * divide this point by a scalar
		 *
		 * @param divisor what to divide the point by
		 * @return reference to this point after division
		 */
		FIntPoint& operator/=(int32_t divisor);

		/*
		 * add other point component-wise to this point
		 *
		 * @param other the point to add to this point
		 * @return reference to this point after addition
		 */
		FIntPoint& operator+=(const FIntPoint& other);

		/*
		 * multiply another point component-wise from this point
		 *
		 * @param other the point to multiply with this point
		 * @return reference to this point after multiplication
		 */
		FIntPoint& operator*=(const FIntPoint& other);

		/*
		 * subtract another point component-wise from this point
		 *
		 * @param other the point to subtract from this point
		 * @return reference to this point after subtraction
		 */
		FIntPoint& operator-=(const FIntPoint& other);

		/*
		 * divide this point component-wise by another point
		 *
		 * @param other the point to divide with
		 * @return reference to this point after division
		 */
		FIntPoint& operator/=(const FIntPoint& other);

		/*
		 * assign another point to this one
		 *
		 * @param other the point to assign this point from
		 * @return reference to this point after assignment
		 */
		FIntPoint& operator=(const FIntPoint& other);

		/*
		 * get the result of scaling on this point
		 *
		 * @param scale what to multiply the point by
		 * @return a new scaled int point
		 */
		FIntPoint operator*(int32_t Scale) const;

		/*
		 * get the result of division on this point
		 *
		 * @param divisor what to divide the point by
		 * @return A new divided int point
		 */
		FIntPoint operator/(int32_t Divisor) const;

		/*
		 * get the result of addition on this point
		 *
		 * @param other the other point to add to this
		 * @return a new combined int point
		 */
		FIntPoint operator+(const FIntPoint& other) const;

		/*
		 * get the result of subtraction from this point
		 *
		 * @param other the other point to subtract from this
		 * @return a new subtracted int point
		 */
		FIntPoint operator-(const FIntPoint& other) const;

		/*
		 * get the result of multiplication on this point
		 *
		 * @param other the point to multiply with this point
		 * @return a new multiplied int point
		 */
		FIntPoint operator*(const FIntPoint& other) const;

		/*
		 * get the result of division on this point
		 *
		 * @param other the other point to subtract from this
		 * @return a new subtracted int point
		 */
		FIntPoint operator/(const FIntPoint& other) const;

		/*
		 * get specific component of the point
		 *
		 * @param index the index of point component
		 * @return reference to component
		 */
		int32_t& operator[](int32_t index);

		/*
		 * get specific component of the point
		 *
		 * @param index the index of point component
		 * @return copy of component value
		 */
		int32_t operator[](int32_t index) const;

	public:
		/*
		 * get the component-wise min of two points
		 *
		 * @see ComponentMax, GetMax
		 */
		FIntPoint ComponentMin(const FIntPoint& other) const;

		/*
		 * get the component-wise max of two points
		 *
		 * @see ComponentMin, GetMin
		 */
		FIntPoint ComponentMax(const FIntPoint& other) const;

		/*
		 * get the larger of the point's two components
		 *
		 * @return the maximum component of the point
		 * @see GetMin, Size, SizeSquared
		 */
		int32_t GetMax() const;

		/*
		 * get the smaller of the point's two components
		 *
		 * @return the minimum component of the point
		 * @see GetMax, Size, SizeSquared
		 */
		int32_t GetMin() const;

		/*
		 * get the distance of this point from (0, 0)
		 *
		 * @return the distance of this point from (0, 0)
		 * @see GetMax, GetMin, SizeSquared
		 */
		int32_t Size() const;

		/*
		 * get the squared distance of this point from (0, 0)
		 *
		 * @return the squared distance of this point from (0, 0)
		 * @see GetMax, GetMin, Size
		 */
		int32_t SizeSquared() const;

		/*
		 * get a textual representation of this point
		 *
		 * @return a string describing the point
		 */
		std::string ToString() const;

	public:
		/*
		 * divide an int point and round up the result
		 *
		 * @param lhs the int point begin divided
		 * @param divisor what to divide the int point by
		 * @return a new divided int point
		 * @see DivideAndRoundDown
		 */
		static FIntPoint DivideAndRoundUp(FIntPoint lhs, int32_t divisor);

		static FIntPoint DivideAndRoundUp(FIntPoint lhs, FIntPoint divisor);

		/*
		 * divide an int point and round down the result
		 *
		 * @param lhs the int point being divided
		 * @param divisor what to divide the int point by
		 * @return a new divided int point
		 * @see DivideAndRoundUp
		 */
		static FIntPoint DivideAndRoundDown(FIntPoint lhs, int32_t divisor);

		/*
		 * get number of components point has
		 *
		 * @return number of components point has
		 */
		static int32_t Num();

	public:
		
		 //todo:implement serialize function


	};

	/*------FIntPoint inline functions------*/
	inline FIntPoint::FIntPoint()
	{
	}

	inline FIntPoint::FIntPoint(int32_t InX, int32_t InY)
		: X(InX)
		, Y(InY)
	{
	}

	inline FIntPoint::FIntPoint(int32_t InXY)
		: X(InXY)
		, Y(InXY)
	{
	}

	inline const int32_t& FIntPoint::operator()(int32_t point_index) const
	{
		return XY[point_index];
	}

	inline int32_t& FIntPoint::operator()(int32_t point_index)
	{
		return XY[point_index];
	}

	inline bool FIntPoint::operator==(const FIntPoint& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	inline bool FIntPoint::operator!=(const FIntPoint& Other) const
	{
		return (X != Other.X) || (Y != Other.Y);
	}

	inline FIntPoint& FIntPoint::operator*=(int32_t scale)
	{
		X *= scale;
		Y *= scale;

		return *this;
	}

	inline FIntPoint& FIntPoint::operator/=(int32_t Divisor)
	{
		X /= Divisor;
		Y /= Divisor;

		return *this;
	}

	inline FIntPoint& FIntPoint::operator+=(const FIntPoint& Other)
	{
		X += Other.X;
		Y += Other.Y;

		return *this;
	}

	inline FIntPoint& FIntPoint::operator*=(const FIntPoint& Other)
	{
		X *= Other.X;
		Y *= Other.Y;

		return *this;
	}

	inline FIntPoint& FIntPoint::operator-=(const FIntPoint& Other)
	{
		X -= Other.X;
		Y -= Other.Y;

		return *this;
	}

	inline FIntPoint& FIntPoint::operator/=(const FIntPoint& other)
	{
		X /= other.X;
		Y /= other.Y;

		return *this;
	}

	inline FIntPoint& FIntPoint::operator=(const FIntPoint& Other)
	{
		X = Other.X;
		Y = Other.Y;

		return *this;
	}

	inline FIntPoint FIntPoint::operator*(int32_t Scale) const
	{
		return FIntPoint(*this) *= Scale;
	}

	inline FIntPoint FIntPoint::operator/(int32_t Divisor) const
	{
		return FIntPoint(*this) /= Divisor;
	}

	inline FIntPoint FIntPoint::operator+(const FIntPoint& other) const
	{
		return FIntPoint(*this) += other;
	}

	inline FIntPoint FIntPoint::operator-(const FIntPoint& other) const
	{
		return FIntPoint(*this) -= other;
	}

	inline FIntPoint FIntPoint::operator*(const FIntPoint& other) const
	{
		return FIntPoint(*this) *= other;
	}

	inline FIntPoint FIntPoint::operator/(const FIntPoint& other) const
	{
		return FIntPoint(*this) /= other;
	}

	inline int32_t& FIntPoint::operator[](int32_t index)
	{
		//todo:check index
		return ((index == 0) ? X : Y);
	}

	inline int32_t FIntPoint::operator[](int32_t index) const
	{
		//todo:check index
		return ((index == 0) ? X : Y);
	}

	inline FIntPoint FIntPoint::ComponentMin(const FIntPoint& other) const
	{
		return FIntPoint(std::min(X, other.X), std::min(Y, other.Y));
	}

	inline FIntPoint FIntPoint::ComponentMax(const FIntPoint& other) const
	{
		return FIntPoint(std::max(X, other.X), std::max(Y, other.Y));
	}

	inline int32_t FIntPoint::GetMax() const
	{
		return std::max(X, Y);
	}

	inline int32_t FIntPoint::GetMin() const
	{
		return std::min(X, Y);
	}

	inline int32_t FIntPoint::Size() const
	{
		int64_t x64 = (int64_t)X;
		int64_t y64 = (int64_t)Y;

		return int32_t(std::sqrt(float(x64 * x64 + y64 * y64)));
	}

	inline int32_t FIntPoint::SizeSquared() const
	{
		return X * X + Y * Y;
	}

	inline std::string FIntPoint::ToString() const
	{
		//todo:need to implement printf to string
		return std::string();
	}

	inline FIntPoint FIntPoint::DivideAndRoundUp(FIntPoint lhs, int32_t divisor)
	{
		//todo:need to implement math's divide and round up
		return FIntPoint();
	}

	inline FIntPoint FIntPoint::DivideAndRoundUp(FIntPoint lhs, FIntPoint divisor)
	{
		return FIntPoint();
	}

	inline FIntPoint FIntPoint::DivideAndRoundDown(FIntPoint lhs, int32_t divisor)
	{
		return FIntPoint();
	}

	inline int32_t FIntPoint::Num()
	{
		return 2;
	}
	/*------FIntPoint inline functions------*/
}
