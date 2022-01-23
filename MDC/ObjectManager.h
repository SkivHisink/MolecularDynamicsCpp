#pragma once
#include <vector>
#include "PhysObject.h"


class ObjectManager final
{
	std::vector<PhysObject***> data;
	const int x_obj_numb = 10; // начальное кол-во объектов по оси X
	const double x_obj_step = 10; // расстояние между объектами на оси X
	const int y_obj_numb = 10;// начальное кол-во объектов по оси Y
	const double y_obj_step = 10; // расстояние между объектами на оси Y
	const int z_obj_numb = 10;// начальное кол-во объектов по оси Z
	const double z_obj_step = 10; // расстояние между объектами на оси Z

	void create_objects();
	void connect_objects();
public:
	ObjectManager(const int x_obj_numb_, const double x_obj_step_,
		const int y_obj_numb_, const double y_obj_step_,
		const int z_obj_numb_, const double z_obj_step_) :
		x_obj_numb(x_obj_numb_), x_obj_step(x_obj_step_),
		y_obj_numb(y_obj_numb_), y_obj_step(y_obj_step_),
		z_obj_numb(z_obj_numb_), z_obj_step(z_obj_step_);

	void delete_all_connections(PhysObject* obj);
	bool delete_object(const int i_, const int j_, const int k_);
	bool delete_objects(const int i_begin, const int i_end,
		const int j_begin, const int j_end,
		const int k_begin, const int k_end);

	PhysObject* get_obj(int i, int j, int k);
	int get_x_obj_numb() const;
	double get_x_obj_step() const;

	int get_y_obj_numb() const;
	double get_y_obj_step() const;

	int get_z_obj_numb() const;
	double get_z_obj_step() const;

	~ObjectManager();
};