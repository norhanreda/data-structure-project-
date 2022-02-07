#ifndef __FORMUALTION_EVENT_H_
#define __FORMULATION_EVENT_H_

#pragma once
#include "../mars_exploration/Event.h"

class FormulationEvent : 
    public Event
{
private:
    type Mission_Type;
    double Target_Location;
    int Mission_Duration;
    int Significance;

public:

    FormulationEvent();
    ~FormulationEvent();

    FormulationEvent(type T,int ED,int ID, double TL, int MD, int Sig);
    void Execute(MarsStation * pStation);

};

#endif

