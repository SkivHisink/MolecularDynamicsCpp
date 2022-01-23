#pragma once
#include "Vector3.h"


class MyObject final
{
public:
	Vector3 Force;
	int ObjectPosition_1;
	int ObjectPosition_2;
	Vector3 Speed;
	Vector3 location;
	void* Parent; // PhysObject
	MyObject()
	{

	}
};