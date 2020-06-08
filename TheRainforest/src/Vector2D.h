#pragma once

struct Vector2D
{
	float x, y;
	Vector2D();
	Vector2D(float x, float y);
	Vector2D Normalize();
	Vector2D MoveTowards(Vector2D to, float step);
	Vector2D Clamp(float min, float max);
	bool IsZero();

	void Print();
};