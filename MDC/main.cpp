#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <fstream>
#include <iostream>
#include <valarray>
#include <vector>

class Vector3 final
{
private:
	double x;
	double y;
	double z;
public:
	double get_X()
	{
		return x;
	}
	double get_const_X() const
	{
		return x;
	}
	double set_X(double& val)
	{
		x = val;
	}
	double get_Y()
	{
		return y;
	}
	double get_const_Y() const
	{
		return y;
	}
	double set_Y(double& val)
	{
		y = val;
	}
	double get_Z()
	{
		return z;
	}
	double get_const_Z() const
	{
		return z;
	}
	double set_Z(double& val)
	{
		z = val;
	}
	Vector3(double x, double y, double z) :x(x), y(y), z(z) {}
	Vector3() :x(0.), y(0.), z(0.) {}
	double magnitude() const
	{
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
	}
	Vector3 operator+(const Vector3& vec) const
	{
		return { this->x + vec.x, this->y + vec.y, this->z + vec.z };
	}
	void operator+=(const Vector3& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
	}
	Vector3 operator-(const Vector3& vec)const
	{
		return { this->x - vec.x, this->y - vec.y, this->z - vec.z };
	}
	Vector3 operator*(const double& val) const
	{
		return { this->x * val, this->y * val, this->z * val };
	}
	Vector3 normalized() const
	{
		double norm_val = this->magnitude();
		norm_val = 1. / norm_val;
		return Vector3(this->x * norm_val, this->y * norm_val, this->z * norm_val);
	}
	static Vector3 right()
	{
		return Vector3(1., 0., 0.);
	}
	static Vector3 left()
	{
		return Vector3(-1., 0., 0.);
	}
	static Vector3 up()
	{
		return Vector3(0., 1., 0.);
	}
	static Vector3 down()
	{
		return Vector3(0., -1., 0.);
	}
	static Vector3 forward()
	{
		return Vector3(0., 0., 1.);
	}
	static Vector3 back()
	{
		return Vector3(0., 0., -1.);
	}
	static Vector3 zero()
	{
		return Vector3(0., 0., 0.);
	}

};

std::ostream& operator<< (std::ostream& out, const Vector3& point)
{
	out << point.get_const_X() << " " << point.get_const_Y() << " " << point.get_const_Z();
	return out;
}

class MySphere
{
public:
	Vector3 Force;
	int ObjectPosition_1;
	int ObjectPosition_2;
	Vector3 Speed;
	Vector3 location;
	void* Parent; // PhysObject
	MySphere()
	{

	}
};

class PhysObject
{
public:
	MySphere* obj_;
	MySphere* left;
	MySphere* right;
	MySphere* front;
	MySphere* back;
	MySphere* up;
	MySphere* down;
	PhysObject() {}
	PhysObject(MySphere* new_obj)
	{
		obj_ = new_obj;
		obj_->Parent = this;
	}
	~PhysObject()
	{
		delete obj_;
	}
};

class MSDSolver final
{
	int XObjectNumber = 10;
	double XObjectStep = 10;
	int YObjectNumber = 10;
	double YObjectStep = 10;
	int ZObjectNumber = 10;
	double ZObjectStep = 10;
	double K = 1.2;
	double Mass = 0.12;
	double C = 2;
	double Pe = 0.;
	double Ke = 0.;
	double Pe_Summ = 0.;
	double Ke_Summ = 0.;
	double Summ = 0.;
	double distance;
	bool _full_energy = false;
	double full_energy = 0;
	std::vector<PhysObject***> ObjectsArray;
	int population = XObjectNumber * YObjectNumber * ZObjectNumber;
	Vector3* matrices = new Vector3[population];
	std::vector<double> keneticEnergy;
	std::vector<double> potentialEnergy;
	std::vector<double> speed; //DEBUG!
	double deltaMax = 0.;
	std::ofstream coord;
	void ObjectCreating()
	{
		for (int i = 0; i < XObjectNumber; ++i)
		{
			ObjectsArray.push_back(new PhysObject * *[YObjectNumber]);
			for (int j = 0; j < YObjectNumber; ++j)
			{
				ObjectsArray[i][j] = new PhysObject * [ZObjectNumber];
			}
		}
		for (int i = 0; i < XObjectNumber; ++i)
		{
			for (int j = 0; j < YObjectNumber; ++j)
			{
				for (int k = 0; k < ZObjectNumber; ++k)
				{
					ObjectsArray[i][j][k] = new PhysObject(new MySphere());
					ObjectsArray[i][j][k]->obj_->location = Vector3(i * XObjectStep, j * YObjectStep, k * ZObjectStep);
				}
			}
		}
	}


	void SetForce()
	{
		//for (int i = 0; i < XObjectNumber - 1; ++i)
		for (int j = 0; j < YObjectNumber; ++j)
			for (int k = 0; k < ZObjectNumber; ++k)
			{
				ObjectsArray[0][j][k]->obj_->Force = Vector3::right() * 50;
				//ObjectsArray[1][0][0].obj_.Force = Vector3.right * 100;
				//ObjectsArray[0][0][1].obj_.Force = Vector3.right * 1;
			}
	}

	void ObjectConnecting()
	{
		for (int i = 0; i < XObjectNumber; ++i)
		{
			for (int j = 0; j < YObjectNumber; ++j)
			{
				for (int k = 0; k < ZObjectNumber; ++k)
				{
					if (i > 0)
					{
						ObjectsArray[i][j][k]->left = ObjectsArray[i - 1][j][k]->obj_;
					}
					if (i < XObjectNumber - 1)
					{
						ObjectsArray[i][j][k]->right = ObjectsArray[i + 1][j][k]->obj_;
					}
					if (j > 0)
					{
						ObjectsArray[i][j][k]->back = ObjectsArray[i][j - 1][k]->obj_;
					}
					if (j < YObjectNumber - 1)
					{
						ObjectsArray[i][j][k]->front = ObjectsArray[i][j + 1][k]->obj_;
					}
					if (k > 0)
					{
						ObjectsArray[i][j][k]->down = ObjectsArray[i][j][k - 1]->obj_;
					}
					if (k < ZObjectNumber - 1)
					{
						ObjectsArray[i][j][k]->up = ObjectsArray[i][j][k + 1]->obj_;
					}
				}
			}
		}
	}
public:
	void Start()
	{
		coord.open("coord.csv");
		coord << XObjectNumber << " " << YObjectNumber << " " << ZObjectNumber << std::endl;
		population = XObjectNumber * YObjectNumber * ZObjectNumber;
		//create objects
		ObjectCreating();
		//connect objects
		ObjectConnecting();
		// setting forces
		SetForce();
		distance = (matrices[0] - matrices[population - 1]).magnitude();
		auto deltaX = distance;
		auto RestoringForce = -(distance - deltaX) * K;
	}

	void Solve(double delta, double endTime)
	{
		coord << endTime / delta << std::endl;
		double beginTime = 0.;
		int counter = 0;
		while (beginTime < endTime)
		{
			for (int i = 0; i < XObjectNumber - 1; ++i)
			{
				for (int j = 0; j < YObjectNumber; ++j)
				{
					for (int k = 0; k < ZObjectNumber; ++k)
					{
						auto obj = ObjectsArray[i][j][k];
						obj->obj_->Speed += obj->obj_->Force * (delta / Mass);
						Ke += pow(obj->obj_->Speed.magnitude(), 2) * Mass * 0.5;
						auto t = obj->obj_->Speed * delta;
						obj->obj_->location += t;
						matrices[i * YObjectNumber * ZObjectNumber + j * ZObjectNumber + k] = obj->obj_->location;
					}
				}
			}
			if (!_full_energy)
			{
				full_energy = Ke;
				_full_energy = true;
			}
			speed.push_back(ObjectsArray[0][0][0]->obj_->Speed.get_X());
			for (int i = 0; i < XObjectNumber - 1; ++i)
			{
				for (int j = 0; j < YObjectNumber; ++j)
				{
					for (int k = 0; k < ZObjectNumber; ++k)
					{
						auto obj = ObjectsArray[i][j][k];
						obj->obj_->Force = Vector3::zero();
						if (obj->right != nullptr)
						{
							auto diff = obj->right->location - obj->obj_->location;
							double x = diff.magnitude() - XObjectStep;
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
							double x = diff.magnitude() - XObjectStep;
							obj->obj_->Force += diff.normalized() * K * x;
						}
						if (obj->front != nullptr)
						{
							auto diff = obj->front->location - obj->obj_->location;
							double y = diff.magnitude() - YObjectStep;
							obj->obj_->Force += diff.normalized() * K * y;
							Pe += pow(y, 2) * 0.5 * K;
						}
						if (obj->back != nullptr)
						{
							auto diff = obj->back->location - obj->obj_->location;
							double y = diff.magnitude() - YObjectStep;
							obj->obj_->Force += diff.normalized() * K * y;
						}
						if (obj->up != nullptr)
						{
							auto diff = obj->up->location - obj->obj_->location;
							double z = diff.magnitude() - ZObjectStep;
							obj->obj_->Force += diff.normalized() * K * z;
							Pe += pow(z, 2) * 0.5 * K;
						}
						if (obj->down != nullptr)
						{
							auto diff = obj->down->location - obj->obj_->location;
							double z = diff.magnitude() - ZObjectStep;
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
				SaveCoordinates();
			}
			beginTime += delta;
		}
	}

	void SaveCoordinates()
	{
		for (int i = 0; i < XObjectNumber; ++i)
		{
			for (int j = 0; j < YObjectNumber; ++j)
			{
				for (int k = 0; k < ZObjectNumber; ++k)
				{
					coord << ObjectsArray[i][j][k]->obj_->location << std::endl;
				}
			}
		}
		coord << std::endl;
	}

	void SaveData()
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

	~MSDSolver()
	{
		for (int i = 0; i < XObjectNumber; ++i)
		{
			for (int j = 0; j < YObjectNumber; ++j)
			{
				for (int k = 0; k < ZObjectNumber; ++k)
				{
					delete ObjectsArray[i][j][k];
				}
				delete[] ObjectsArray[i][j];
			}
			delete[] ObjectsArray[i];
		}
		delete[] matrices;
		coord.close();
	}
};

int main(void)
{
	if (true)
	{
		MSDSolver solver;
		solver.Start();
		solver.Solve(0.01, 10);
		solver.SaveData();
	}
	return _CrtDumpMemoryLeaks();
}