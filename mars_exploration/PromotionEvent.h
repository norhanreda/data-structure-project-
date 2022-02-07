#ifndef __PROMOTION_EVENT_H_
#define __PROMOTION_EVENT_H_


#include "../mars_exploration/Event.h"
//#include"Mission.h"
//#include "MarsStation.h"

class PromotionEvent : public Event
{
public:

    PromotionEvent();
    PromotionEvent(int ED, int ID);
    ~PromotionEvent();


    void Execute(MarsStation * pStation);
};
#endif

