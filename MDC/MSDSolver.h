#pragma once
#include "ObjectManager.h"

class MSDSolver final
{
	ObjectManager* obj_manager;
	double K = 1.2; // коэффициент упругости
	double Mass = 0.12; // масса шариков
	double Pe = 0.; // потенциальная энергия за dt
	double Ke = 0.; // кинетическая энергия за dt
	double Pe_Summ = 0.; // суммарная потенциальная энергия
	double Ke_Summ = 0.; // суммарная кинетическая энергия
	bool _full_energy = false;
	double full_energy = 0;
	int population;
	Vector3* matrices;
	std::vector<double> keneticEnergy;
	std::vector<double> potentialEnergy;
	std::vector<double> speed; //DEBUG!
	double deltaMax = 0.;
	std::ofstream coord;

	void SetForce();
public:
	MSDSolver(ObjectManager* obj_manager_);
	void Start();

	void Solve(double delta, double endTime);
	void SaveCoordinates();;

	void SaveData();

	~MSDSolver();
};