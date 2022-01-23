#include "MSDSolver.h"

void MSDSolver::SetForce()
{
	//for (int i = 0; i < XObjectNumber - 1; ++i)
	for (int j = 0; j < obj_manager->get_y_obj_numb(); ++j)
		for (int k = 0; k < obj_manager->get_z_obj_numb(); ++k)
		{
			obj_manager->get_obj(0, j, k)->obj_->Force = Vector3::right() * 50;
		}
}
MSDSolver::MSDSolver(ObjectManager* obj_manager_)
{
	obj_manager = obj_manager_;
	population = obj_manager->get_x_obj_numb() * obj_manager->get_y_obj_numb() * obj_manager->get_z_obj_numb();
	matrices = new Vector3[population];
}
void MSDSolver::Start()
{
	coord.open("coord.csv"); // открываем файл для отрисовки
	coord << obj_manager->get_x_obj_numb() << " " <<
		obj_manager->get_y_obj_numb() << " " <<
		obj_manager->get_z_obj_numb() << std::endl;//записываем максимально возможное кол-во объектов
	// setting forces
	SetForce();
	double distance = (matrices[0] - matrices[population - 1]).magnitude();
	auto deltaX = distance;
	auto RestoringForce = -(distance - deltaX) * K;
}

void MSDSolver::Solve(double delta, double endTime)
{
	coord << endTime / delta << std::endl; // записываем количество шагов, которые мы записываем
	int counter = 0;
	for (double beginTime = 0.; beginTime < endTime; beginTime += delta)
	{
		{
			for (int i = 0; i < obj_manager->get_x_obj_numb() - 1; ++i)
			{
				for (int j = 0; j < obj_manager->get_y_obj_numb(); ++j)
				{
					for (int k = 0; k < obj_manager->get_z_obj_numb(); ++k)
					{
						auto obj = obj_manager->get_obj(i, j, k);
						obj->obj_->Speed += obj->obj_->Force * (delta / Mass);
						Ke += pow(obj->obj_->Speed.magnitude(), 2) * Mass * 0.5;
						auto t = obj->obj_->Speed * delta;
						obj->obj_->location += t;
						matrices[i * obj_manager->get_z_obj_numb() * obj_manager->get_y_obj_numb() + j * obj_manager->get_z_obj_numb() + k] = obj->obj_->location;
					}
				}
			}
			if (!_full_energy)
			{
				full_energy = Ke;
				_full_energy = true;
			}
			speed.push_back(obj_manager->get_obj(0, 0, 0)->obj_->Speed.get_X());
			for (int i = 0; i < obj_manager->get_x_obj_numb() - 1; ++i)
			{
				for (int j = 0; j < obj_manager->get_y_obj_numb(); ++j)
				{
					for (int k = 0; k < obj_manager->get_z_obj_numb(); ++k)
					{
						auto obj = obj_manager->get_obj(i, j, k);
						obj->obj_->Force = Vector3::zero();
						if (obj->right != nullptr)
						{
							auto diff = obj->right->location - obj->obj_->location;
							double x = diff.magnitude() - obj_manager->get_x_obj_step();
							obj->obj_->Force += diff.normalized() * K * x;
							Pe += pow(x, 2) * 0.5 * K;
							if (deltaMax > x)
							{
								deltaMax = x;
							}
						}
						if (obj->left != nullptr)
						{
							auto diff = obj->left->location - obj->obj_->location;
							double x = diff.magnitude() - obj_manager->get_x_obj_step();
							obj->obj_->Force += diff.normalized() * K * x;
						}
						if (obj->front != nullptr)
						{
							auto diff = obj->front->location - obj->obj_->location;
							double y = diff.magnitude() - obj_manager->get_y_obj_step();
							obj->obj_->Force += diff.normalized() * K * y;
							Pe += pow(y, 2) * 0.5 * K;
						}
						if (obj->back != nullptr)
						{
							auto diff = obj->back->location - obj->obj_->location;
							double y = diff.magnitude() - obj_manager->get_y_obj_step();
							obj->obj_->Force += diff.normalized() * K * y;
						}
						if (obj->up != nullptr)
						{
							auto diff = obj->up->location - obj->obj_->location;
							double z = diff.magnitude() - obj_manager->get_z_obj_step();
							obj->obj_->Force += diff.normalized() * K * z;
							Pe += pow(z, 2) * 0.5 * K;
						}
						if (obj->down != nullptr)
						{
							auto diff = obj->down->location - obj->obj_->location;
							double z = diff.magnitude() - obj_manager->get_z_obj_step();
							obj->obj_->Force += diff.normalized() * K * z;
						}
					}
				}
				Pe_Summ += Pe;
				Ke_Summ += Ke;
				Pe = Ke = 0.;
			}

			if (abs(beginTime - counter * 0.1) < 0.1)
			{
				if (counter != 0)
				{
					keneticEnergy.push_back(Ke_Summ);
					potentialEnergy.push_back(Pe_Summ);
				}
				Ke_Summ = 0;
				Pe_Summ = 0;
				++counter;
				//SaveCoordinates();
			}
		}
	}
}
void MSDSolver::SaveCoordinates()
{
	for (int i = 0; i < obj_manager->get_x_obj_numb(); ++i)
	{
		for (int j = 0; j < obj_manager->get_y_obj_numb(); ++j)
		{
			for (int k = 0; k < obj_manager->get_z_obj_numb(); ++k)
			{
				coord << obj_manager->get_obj(i, j, k)->obj_->location << std::endl;
			}
		}
	}
	coord << std::endl;
}

void MSDSolver::SaveData()
{
	std::ofstream file;
	file.open("data.csv");
	file << "kenetic,potential" << std::endl;
	for (int i = 0; i < keneticEnergy.size(); ++i)
	{
		file << keneticEnergy[i] << "," << potentialEnergy[i] << std::endl;
	}
	file.close();
	file.open("speed.csv");
	file << "speed" << std::endl;
	for (int i = 0; i < speed.size(); ++i)
	{
		file << speed[i] << std::endl;
	}
	file.close();
}

MSDSolver::~MSDSolver()
{
	delete[] matrices;
	coord.close();
}