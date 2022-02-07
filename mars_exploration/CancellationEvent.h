#ifndef __CANCELLATION_EVENT_H_
#define __CANCELLATION_EVENT_H_

#pragma once
#include "../mars_exploration/Event.h"

class CancellationEvent : public Event
{

public:
    CancellationEvent();
    ~CancellationEvent();
    CancellationEvent(int ED, int ID);
    void Execute(MarsStation * pStation);
};
#endif

