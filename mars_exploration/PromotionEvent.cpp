#include "../mars_exploration/PromotionEvent.h"
#include "../mars_exploration/MarsStation.h"


PromotionEvent::PromotionEvent()
{
}

PromotionEvent::PromotionEvent(int ED, int ID)
{
	setED(ED);
	setID(ID);
}

PromotionEvent::~PromotionEvent()
{
}

void PromotionEvent::Execute(MarsStation* pStation)
{
	pStation->PromoteMountainousMission(Mission_ID);
}
