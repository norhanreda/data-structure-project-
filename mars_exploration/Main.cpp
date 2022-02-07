#include "MarsStation.h"

int main()
{
	MarsStation* pMarsStation = new MarsStation;
	pMarsStation->Simulate();

	delete pMarsStation;
	pMarsStation = NULL;

	return 0;
}