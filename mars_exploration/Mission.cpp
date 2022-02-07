#include"Mission.h"
#include <cmath>

Mission::Mission(int Id, int FD, type Type, double TL, int MD, int Sig)
{
	setID(Id);
	setMT(Type);
	setFD(FD);
	setTL(TL);
	setMD(MD);
	setSIG(Sig);
	SetStatus(Waiting);
	Waiting_Time = 0;


}

Mission::Mission() {}

void Mission::setID(int id)
{
	ID=id;
}

int Mission::getID()
{
	return ID;
}

void Mission::setFD(int FD)	//Formulation_Day
{
	Formulation_Day=FD;
}

int Mission::getFD()		//Formulation_Day
{
	return Formulation_Day;
}

void Mission::setCD(int CD)	//Completion Day
{
	Completion_Day = CD;
}

int Mission::getCD()		//Completion Day
{
	return Completion_Day;
}

void Mission::setED(int ED)
{
	Execution_Days = ED;
}

int Mission::getED()
{
	return Execution_Days;
}

void Mission::setTL(double TL)//Target_Location
{
	Target_Location=TL;
}

int Mission::getTL()//Target_Location
{
	return Target_Location;
}

void Mission::setMD(int MD)//Mission_Duration
{
	Mission_Duration=MD;
}

int Mission::getMD()//Mission_Duration
{
	return Mission_Duration;
}

void Mission::setMT(type T)// Mission_Type
{
	Mission_Type=T;
}

type Mission::getMT() //Mission_Type
{
	return Mission_Type;
}

void Mission::setSIG(int Sig)//Significance
{
	Significance=Sig;
}

int Mission::getSIG()//Significance
{
	return Significance;
}

int Mission::calc_Execution_time(int speed)
{
	return ceil(2 * (Target_Location / (speed * 25.0)) + Mission_Duration);
}

int Mission::calc_Completion_time(int speed)
{
	//if (status == compleated)
		return Formulation_Day + calc_Execution_time(speed) + get_Waiting_Time();
}

void Mission::set_Waiting_Time(int wait)
{
	Waiting_Time = wait;
}
int Mission::get_Waiting_Time()
{
	return Waiting_Time;
}

void Mission::SetStatus(MISSION_STATUS s)
{
	status = s;
}

MISSION_STATUS Mission::GetStatus() const
{
	return status;
}
