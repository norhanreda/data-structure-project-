#include "CancellationEvent.h"
#include "../mars_exploration/MarsStation.h"

CancellationEvent::CancellationEvent(int ED, int ID)
{
	setID(ID);
	setED(ED);
}

//Calls function in mars station to cancel a specific mission
void CancellationEvent::Execute(MarsStation * pStation)
{
	pStation->CancelMountainousMission(Mission_ID);
}
