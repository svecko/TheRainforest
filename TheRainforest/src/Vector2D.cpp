#include <cmath>
#include <iostream>

#include "Vector2D.h"

Vector2D::Vector2D()
{
}

Vector2D::Vector2D(float x, float y)
	: x(x), y(y)
{
}

Vector2D Vector2D::Normalize()
{
	if (this->x == 0.0f && this->y == 0.0f)
	{
		return  Vector2D(0.0f, 0.0f);
	}
	else
	{
		float magnitude = sqrt(this->x * this->x + this->y * this->y);
		return Vector2D(this->x / magnitude, this->y / magnitude);
	}
}

Vector2D Vector2D::MoveTowards(Vector2D to, float step)
{
	Vector2D a;
	a.x = to.x - this->x;
	a.y = to.y - this->y;

	float magnitude = sqrt(a.x * a.x + a.y * a.y);

	if (magnitude <= step || magnitude == 0.0f)
	{
		return Vector2D(to.x, to.y);
	}
	return Vector2D(this->x + a.x / magnitude * step, this->y + a.y / magnitude * step);
}

Vector2D Vector2D::Clamp(float min, float max)
{
	if (this->x < min)
	{
		this->x = min;
	}
	if (this->x > max)
	{
		this->x = max;
	}
	if (this->y < min)
	{
		this->y = min;
	}
	if (this->y > max)
	{
		this->y = max;
	}

	return Vector2D(this->x, this->y);
}

bool Vector2D::IsZero()
{
	return this->x == 0.0f && this->y == 0.0f;
}

void Vector2D::Print()
{
	std::cout << "x: " << x << " y: " << y << std::endl;
}
