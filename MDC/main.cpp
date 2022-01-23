#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <valarray>


int main(void)
{
	if (true)
	{
		auto obj_manager = new ObjectManager(
			10, 10,
			10, 10,
			10, 10);
		MSDSolver solver(obj_manager);
		solver.Start();
		solver.Solve(0.001, 10);
		solver.SaveData();
		delete obj_manager;
	}
	return _CrtDumpMemoryLeaks();
}