#include "rovers.h"

rovers::rovers()
{
}

rovers::rovers(int ID, type Type, int speed, int Mission_Checkup, int Checkup_Duration)
{
	roverID = ID;
	roverType = Type;
	roverSpeed = speed;
	N_beforecheckup = Mission_Checkup;
	checkupDuration = Checkup_Duration;
	Executed_missions = 0;
	isAvailable = true;
	isAssigned = false;
	isUnderCheckup = false;
	missionOfrover = nullptr;
}

rovers::~rovers()
{
}

void rovers::setroverID(int id)
{
	roverID = id;
}

int rovers::getroverID()
{
	return roverID;
}

void rovers::setroverType(type type)
{
	roverType = type;
}

type rovers::getroverType()
{
	return roverType;
}

void rovers::setroverSpeed(int speed)
{
	roverSpeed = speed;
}

int rovers::getroverSpeed()
{
	return roverSpeed;
}

void rovers::setcheckupDuration(int duration)
{
	checkupDuration = duration;
}

int rovers::getcheckupDuration()
{
	return checkupDuration;
}

void rovers::setN(int n)
{
	N_beforecheckup = n;
}

int rovers::getN()
{
	return N_beforecheckup;
}


/////////////////////////
void rovers::set_IsAvailable(bool isAva)
{
	isAvailable = isAva;
}
void rovers::set_IsAssigned(bool isAssig)
{
	isAssigned = isAssig;
}
void rovers::set_IsUnderCheckUP(bool ischeck)
{
	isUnderCheckup = ischeck;
}
bool rovers::get_IsAvailable()
{
	return isAvailable;
}
bool rovers::get_IsAssigned()
{
	return isAssigned;
}
bool rovers::get_IsUnderCheckUP()
{
	return isUnderCheckup;
}
void rovers::setmission(Mission* m)
{
	missionOfrover = m;
	Executed_missions++; 
}
Mission* rovers::getmission()
{
	return missionOfrover;
}
bool rovers::Is_Need_Check_Up()
{
	if (Executed_missions % N_beforecheckup == 0)
		return true;
	return false;
}
void rovers::set_CheckUp_end_Day(int endDay)
{
	CheckUp_end_Day = endDay;
}
int rovers::get_CheckUp_end_Day()
{
	return CheckUp_end_Day;
}