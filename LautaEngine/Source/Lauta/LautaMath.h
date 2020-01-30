#pragma once
#include <iostream>
#include "Debug.h"
#include <time.h>
#include <random>

//vector mathermatics in 2D plane
class Vector2D
{
public:

	float x, y;
	
	//default constructor 
	Vector2D() 
	{ 
		x = 0.f; 
		y = 0.f; 
	};
	Vector2D(float x, float y) 
	{ 
		this->x = x; 
		this->y = y; 
	};


	Vector2D& add(const Vector2D& vec) 
	{ 
		this->x += vec.x; 
		this->y += vec.y;
		return *this;
	};
	Vector2D& subtract(const Vector2D& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		return *this;
	};
	Vector2D& multiply(const Vector2D& vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		return *this;
	};
	Vector2D& divide(const Vector2D& vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
		return *this;
	};

	


	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2) { return v1.add(v2);		};
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2) { return v1.subtract(v2);	};
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2) { return v1.multiply(v2);	};
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2) { return v1.divide(v2);	};

	Vector2D& operator+=(const Vector2D& vec) { return this->add(vec); };
	Vector2D& operator-=(const Vector2D& vec) { return this->subtract(vec); };
	Vector2D& operator*=(const Vector2D& vec) { return this->multiply(vec); };
	Vector2D& operator/=(const Vector2D& vec) { return this->divide(vec); };
	
	Vector2D& operator*(const int& i) 
	{
		this->x *= i;
		this->y *= i; 
		return *this; 
	}

	Vector2D& Zero()
	{
		this->x = 0;
		this->y = 0;
		return *this;
	}

	float vectorLength(const Vector2D& vec)
	{
		return sqrt((vec.x * vec.x) + (vec.y * vec.y));
	}

	Vector2D& Normalize()
	{
		float length = vectorLength(*this);
		this->x = this->x / length;
		this->y = this->y / length;

		return *this;
	}

	Vector2D getDirectionalVector(Vector2D A, Vector2D B)
	{
		Vector2D direction((B.x - A.x), (B.y - A.x));
		return direction.Normalize();
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ")";
		return stream;
	};	
};

class Vector2Di
{
public:

	int x, y;

	//default constructor 
	Vector2Di()
	{
		x = 0;
		y = 0;
	};
	Vector2Di(int x, int y)
	{
		this->x = x;
		this->y = y;
	};

	Vector2Di& add(const Vector2Di& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		return *this;
	};
	Vector2Di& subtract(const Vector2Di& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		return *this;
	};
	Vector2Di& multiply(const Vector2Di& vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		return *this;
	};
	Vector2Di& divide(const Vector2Di& vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
		return *this;
	};

	Vector2Di& Zero()
	{
		this->x = 0;
		this->y = 0;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vector2Di& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ")";
		return stream;
	};
};

inline void drawCircle(int x0, int y0, int radius, DebugDraw* debugDrawer)
{

	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{

		debugDrawer->drawPixel(x0 + x, y0 + y);
		debugDrawer->drawPixel(x0 + y, y0 + x);
		debugDrawer->drawPixel(x0 - y, y0 + x);
		debugDrawer->drawPixel(x0 - x, y0 + y);
		debugDrawer->drawPixel(x0 - x, y0 - y);
		debugDrawer->drawPixel(x0 - y, y0 - x);
		debugDrawer->drawPixel(x0 + y, y0 - x);
		debugDrawer->drawPixel(x0 + x, y0 - y);


		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}


/*
Check if a point is a rectangle of minVec and maxVec

Example: 
_________max
|-----------|
|-----------|
|---point---|		
|-----------|
min_________|

= true

*/
inline bool isPointInRange(Vector2D minVec, Vector2D point, Vector2D maxVec)
{
	if ((minVec.x < point.x && point.x < maxVec.x) && (minVec.y < point.y && point.y < maxVec.y))
	{
		return true;
	}
	else
		return false;
}

inline bool isNumberInRange(int minValue, int number, int maxValue)
{
	if (minValue < number && number < maxValue)
	{
		return true;
	}
	else
		return false;
}

inline int pickRandomNumber(int min, int max)
{
	std::random_device randomNumber;
	std::mt19937 generator(randomNumber());
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

inline Vector2D changeToFloat(const Vector2Di& vec)
{
	return Vector2D((float)vec.x, (float)vec.y);
}

inline Vector2Di changeToInt(const Vector2D& vec)
{
	return Vector2Di((int)vec.x, (int)vec.y);
}

inline Vector2D getUnitVector(Vector2D a, Vector2D b)
{
	//direction vector
	Vector2D dirVec = b.subtract(a);

	float magnitude = sqrt((dirVec.x*dirVec.x) + (dirVec.y*dirVec.y));

	Vector2D unitVec((dirVec.x / magnitude), (dirVec.y / magnitude));

	return unitVec;
}

inline float getAngleOfVector(Vector2D a)
{
	float angle = atan2f(a.y, a.x);
	return (float)(angle * (180 / M_PI));
}

