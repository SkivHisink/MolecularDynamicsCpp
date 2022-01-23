#pragma once
#include "MyObject.h"

class PhysObject final
{
public:
	MyObject* obj_;
	MyObject* left;
	MyObject* right;
	MyObject* front;
	MyObject* back;
	MyObject* up;
	MyObject* down;
	PhysObject() {}
	PhysObject(MyObject* new_obj)
	{
		obj_ = new_obj;
		obj_->Parent = this;
	}
	~PhysObject()
	{
		delete obj_;
	}
};