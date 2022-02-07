#ifndef __EVENT_H_
#define __EVENT_H_

#include "../mars_exploration/Defs.h"

class MarsStation; 

class Event
{

protected:
	int Event_Day;
	int Mission_ID;

public:
	Event();
	Event(int, int);
	void setED(int);
	int getED();
	void setID(int);
	int getID();
	virtual void Execute(MarsStation * pStation) = 0;
};

#endif

