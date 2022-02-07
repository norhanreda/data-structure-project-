#include "FormulationEvent.h"
#include "../mars_exploration/MarsStation.h"



FormulationEvent::FormulationEvent()
{
}

FormulationEvent::~FormulationEvent()
{
}

FormulationEvent::FormulationEvent(type T, int ED, int ID, double TL, int MD, int Sig)
{
	Mission_Type = T;
	setED(ED);
	setID(ID);
	Target_Location = TL;
	Mission_Duration = MD;
	Significance = Sig;
}

void FormulationEvent::Execute(MarsStation * pStation)
{
	//Creating new mission
	Mission* Formulate_Mission = new Mission(Mission_ID, Event_Day, Mission_Type, Target_Location, Mission_Duration, Significance);

	//Adding mission to appropriate list
	if (Formulate_Mission->getMT() == Emergency)
	{
		pStation->AddToQueue(Formulate_Mission, pStation->CalcPriority(Formulate_Mission));
	}
	else
	{
		pStation->AddToQueue(Formulate_Mission);
	}

}
