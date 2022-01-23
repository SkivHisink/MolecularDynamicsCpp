#include "ObjectManager.h"

void ObjectManager::create_objects()
{
	for (int i = 0; i < x_obj_numb; ++i)
	{
		data.push_back(new PhysObject * *[y_obj_numb]);
		for (int j = 0; j < y_obj_numb; ++j)
		{
			data[i][j] = new PhysObject * [z_obj_numb];
		}
	}
	for (int i = 0; i < x_obj_numb; ++i)
	{
		for (int j = 0; j < y_obj_numb; ++j)
		{
			for (int k = 0; k < z_obj_numb; ++k)
			{
				data[i][j][k] = new PhysObject(new MyObject());
				data[i][j][k]->obj_->location = Vector3(i * x_obj_step, j * y_obj_step, k * z_obj_step);
			}
		}
	}
}

void ObjectManager::connect_objects()
{
	for (int i = 0; i < x_obj_numb; ++i)
	{
		for (int j = 0; j < y_obj_numb; ++j)
		{
			for (int k = 0; k < z_obj_numb; ++k)
			{
				if (i > 0)
				{
					data[i][j][k]->left = data[i - 1][j][k]->obj_;
				}
				if (i < x_obj_numb - 1)
				{
					data[i][j][k]->right = data[i + 1][j][k]->obj_;
				}
				if (j > 0)
				{
					data[i][j][k]->back = data[i][j - 1][k]->obj_;
				}
				if (j < y_obj_numb - 1)
				{
					data[i][j][k]->front = data[i][j + 1][k]->obj_;
				}
				if (k > 0)
				{
					data[i][j][k]->down = data[i][j][k - 1]->obj_;
				}
				if (k < z_obj_numb - 1)
				{
					data[i][j][k]->up = data[i][j][k + 1]->obj_;
				}
			}
		}
	}
}
ObjectManager::ObjectManager(const int x_obj_numb_, const double x_obj_step_,
	const int y_obj_numb_, const double y_obj_step_,
	const int z_obj_numb_, const double z_obj_step_) :
	x_obj_numb(x_obj_numb_), x_obj_step(x_obj_step_),
	y_obj_numb(y_obj_numb_), y_obj_step(y_obj_step_),
	z_obj_numb(z_obj_numb_), z_obj_step(z_obj_step_)
{
	create_objects();
	connect_objects();
}

void ObjectManager::delete_all_connections(PhysObject* obj)
{
	if (obj->left != nullptr)
	{
		((PhysObject*)(obj->left->Parent))->right = nullptr;
		obj->left = nullptr;
	}
	if (obj->right != nullptr)
	{
		((PhysObject*)(obj->right->Parent))->left = nullptr;
		obj->right = nullptr;
	}
	if (obj->up != nullptr)
	{
		((PhysObject*)(obj->up->Parent))->down = nullptr;
		obj->up = nullptr;
	}
	if (obj->down != nullptr)
	{
		((PhysObject*)(obj->down->Parent))->up = nullptr;
		obj->down = nullptr;
	}
	if (obj->back != nullptr)
	{
		((PhysObject*)(obj->back->Parent))->front = nullptr;
		obj->back = nullptr;
	}
}

bool ObjectManager::delete_object(const int i_, const int j_, const int k_)
{
	auto obj = data[i_][j_][k_];
	delete_all_connections(obj);
}

bool ObjectManager::delete_objects(const int i_begin, const int i_end,
	const int j_begin, const int j_end,
	const int k_begin, const int k_end)
{
	for (int i = i_begin; i < i_end; ++i)
	{
		for (int j = j_begin; j < j_end; ++j)
		{
			for (int k = k_begin; k < k_end; ++k)
			{
				delete_object(i, j, k);
			}
		}
	}
}

PhysObject* ObjectManager::get_obj(int i, int j, int k)
{
	return data[i][j][k];
}

int ObjectManager::get_x_obj_numb() const
{
	return x_obj_numb;
}

double ObjectManager::get_x_obj_step() const
{
	return x_obj_step;
}

int ObjectManager::get_y_obj_numb() const
{
	return y_obj_numb;
}

double ObjectManager::get_y_obj_step() const
{
	return y_obj_step;
}

int ObjectManager::get_z_obj_numb() const
{
	return z_obj_numb;
}

double ObjectManager::get_z_obj_step() const
{
	return z_obj_step;
}

ObjectManager::~ObjectManager()
{
	for (int i = 0; i < get_x_obj_numb(); ++i)
	{
		for (int j = 0; j < get_y_obj_numb(); ++j)
		{
			for (int k = 0; k < get_z_obj_numb(); ++k)
			{
				delete data[i][j][k];
			}
			delete[] data[i][j];
		}
		delete[] data[i];
	}
}