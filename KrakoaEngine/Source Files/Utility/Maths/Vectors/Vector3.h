#pragma once

template <typename T>
struct VectorBase
{
public:
	explicit VectorBase() : x(0)
	{}

	~VectorBase()
	= default;

	virtual T Magnitude()
	{
		return x;
	}
	
public:
	T x;
};