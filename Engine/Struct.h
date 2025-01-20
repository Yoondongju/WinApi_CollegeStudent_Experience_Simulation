#pragma once

struct Vec2
{
	float x;
	float y;

public:
	bool IsZero()
	{
		if (x == 0.f && y == 0.f)
			return true;
		return false;
	}

	float Length()
	{
		return sqrt(x * x + y * y);

	}

	Vec2& Normalize()
	{
		float fLen = Length();
		
		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}

	Vec2 operator - ()
	{
		return Vec2(-x, -y);
	}


	Vec2 operator +(Vec2 vOther)
	{
		return Vec2(x + vOther.x, y + vOther.y);
	}

	Vec2 operator +=(Vec2 vOther)
	{
		*this = *this + vOther;

		return Vec2(*this);
	}

	Vec2 operator +(float fOther)
	{
		return Vec2(x + fOther, y + fOther);
	}

	Vec2 operator -(Vec2 vOther)
	{
		return Vec2(x - vOther.x, y - vOther.y);
	}

	Vec2 operator -(float _fOther)
	{
		return Vec2(x - _fOther, y - _fOther);
	}

	void operator -= (Vec2 _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}

	void operator -= (float _fOther)
	{
		x -= _fOther;
		y -= _fOther;
	}

	Vec2 operator *(Vec2 vOther)
	{
		return Vec2(x * vOther.x, y * vOther.y);
	}

	Vec2 operator *(int _nOther)
	{
		return Vec2(x * (float)_nOther, y * (float)_nOther);
	}

	Vec2 operator *(float _nOther)
	{
		return Vec2(x * _nOther, y * _nOther);
	}

	Vec2 operator / (Vec2 vOther)
	{
		assert(!(0.f == vOther.x || 0.f == vOther.y));

		return Vec2(x / vOther.x, y / vOther.y);
	}

	Vec2 operator / (float _f)
	{
		assert(! (0.f == _f) );

		return Vec2(x / _f, y / _f);
	}

	Vec2 operator = (POINT pOther)
	{
		return Vec2(x = (float)pOther.x, y = (float)pOther.y);
	}
	

public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(POINT _pOther)
		: x((float)_pOther.x)
		, y((float)_pOther.y)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(int _x, int _y)
		: x((float)_x)
		, y((float)_y)
	{}

	Vec2(float _f)
		: x(_f)
		, y(_f)
	{}

};