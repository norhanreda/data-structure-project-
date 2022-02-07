#include "Event.h"

Event::Event()
{
}

Event::Event(int ED, int ID)
{
	setED(ED);
	setID(Mission_ID);
}

void Event::setED(int ED)
{
	if (ED > 0)
		Event_Day = ED;
	else 
		ED = 0;
}

int Event::getED()
{
	return Event_Day;
}

void Event::setID(int ID)
{
	if (ID > 0)
		Mission_ID = ID;

}

int Event::getID()
{
	return Mission_ID;
}
