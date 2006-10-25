#pragma once

#include <math.h>

template <class T> class TVector2 
{
public:
	TVector2(void);
	TVector2(const T x, const T y);
	TVector2(const TVector2& other);
	TVector2 & operator = ( TVector2 const & other);
	~TVector2(void);

	TVector2 operator + ( TVector2 const & other) const;
	TVector2 operator - ( TVector2 const & other) const;
	TVector2 operator - () const;
	TVector2 operator * ( T const & other) const;
	TVector2 operator / ( TVector2 const & other) const;

	// Dot product
	T operator * ( TVector2 const & other) const;

	TVector2 & operator += ( TVector2 const & other);
	TVector2 & operator *= ( T const & other);
	TVector2 & operator /= ( T const & other);
	TVector2 & operator -= ( TVector2 const & other);

	bool operator == ( TVector2 const & other) const;
	bool operator != ( TVector2 const & other) const;

	// Members Modifiers
	T &x();
	T &y();

	// Accesors
	const T &x() const;
	const T &y() const;

	T Norm() const;

	TVector2& Normalize();

	static T Angle( const TVector2& p1, const TVector2& p2);

	// Angle between unitary vectors
	static T AngleN( const TVector2& p1, const TVector2& p2);

	// Modifiers
	void setX(const T x);
	void setY(const T y);
	void set(const T x, const T y);

protected:
	T iX;
	T iY;
}; // end class

template <class T>
inline 
TVector2<T>::TVector2():
iX(0), iY(0)
{
}

template <class T>
inline 
TVector2<T>::TVector2(const T x, const T y):
iX(x), iY(y)
{
}

template <class T>
inline 
TVector2<T>::TVector2(const TVector2& other)
{
	iX = other.iX;
	iY = other.iY;
}

template <class T>
inline
TVector2<T> & TVector2<T>::operator = ( TVector2 const & other)
{
	iX = other.iX;
	iY = other.iY;

	return *this;
}

template <class T>
inline 
TVector2<T>::~TVector2() {}

template <class T>
inline 
TVector2<T> TVector2<T>::operator - ( TVector2 const & other) const
{ 
	return TVector2( iX - other.iX, iY - other.iY );
}

template <class T>
inline 
TVector2<T> TVector2<T>::operator - () const
{ 
	return TVector2( -iX, -iY );
}

template <class T>
inline 
TVector2<T> TVector2<T>::operator + ( TVector2 const & other) const
{ 
	return TVector2( iX + other.iX, iY + other.iY );
}

template <class T>
inline 
TVector2<T> TVector2<T>::operator * ( T const & other	) const
{ 
	return TVector2( iX * other, iY * other );
}

template <class T>
inline 
TVector2<T> TVector2<T>::operator / ( TVector2 const & other) const
{ 
	return TVector2( iX / other.iX, iY / other.iY );
}

template <class T>
inline 
TVector2<T>& TVector2<T>::operator += ( TVector2 const & other)
{
	iX += other.iX;
	iY += other.iY;
	return *this;
}

template <class T>
inline 
TVector2<T>& TVector2<T>::operator -= ( TVector2 const & other)
{
	iX -= other.iX;
	iY -= other.iY;
	return *this;
}

template <class T>
inline 
TVector2<T>& TVector2<T>::operator *= ( T const & other)
{
	iX *= other;
	iY *= other;
	return *this;
}

template <class T>
inline 
TVector2<T>& TVector2<T>::operator /= ( T const & other)
{
	iX -= other;
	iY -= other;
	return *this;
}

template <class T>
inline
bool TVector2<T>::operator == ( TVector2 const & other) const
{
	return ( (iX == other.iX) && (iY == other.iY) );
}

template <class T>
inline
bool TVector2<T>::operator != ( TVector2 const & other) const
{
	return ( (iX != other.iX) && (iY != other.iY) );
}

template <class T>
inline
const T& TVector2<T>::x() const
{ return iX; }

template <class T>
inline
const T& TVector2<T>::y() const
{ return iY; }


template <class T>
inline
T& TVector2<T>::x()
{ return iX; }

template <class T>
inline
T& TVector2<T>::y()
{ return iY; }

template <class T>
inline
TVector2<T>& TVector2<T>::Normalize()
{
	T norm = this->Norm();
	if( norm > 0.0 )
	{
		iX /= norm;
		iY /= norm;
	}

	return *this;
}

template <class T>
inline
T TVector2<T>::operator * (const TVector2 & p) const
{
	return ( (iX * p.iX) + (iY * p.iY) );
}

template <class T>
inline
T TVector2<T>::Norm() const
{
	return sqrt( iX * iX + iY * iY );
}

template <class T>
inline
T TVector2<T>::Angle( const TVector2& p1, const TVector2& p2)
{
	T w = p1.Norm() * p2.Norm();

	if( w == 0 ) 
	{
		return -1;
	}

	T t = ( p1 * p2 ) / w;

	if( t > 1) 
	{
		t = 1;
	}
	else 
	{
		if( t < -1 )
		{
			t = -1;
		}
	}

	return (T) acos(t);
}

template <class T>
inline
T TVector2<T>::AngleN( const TVector2& p1, const TVector2& p2)
{
	T w = p1 * p2;
	if( w > 1 ) 
	{
		w = 1;
	}
	else
	{
		if( w < -1 ) 
		{
			w =- 1;
		}
	}

	return (T) acos(w);
}

template <class T>
inline
void TVector2<T>::setX(const T x)
{
	iX = x;
}

template <class T>
inline
void TVector2<T>::setY(const T y)
{
	iY = y;
}

template <class T>
inline
void TVector2<T>::set(const T x, const T y)
{
	iX = x;
	iY = y;
}