#ifndef __MISSION_H_
#define __MISSION_H_

#include "Defs.h"

class Mission
{
private:
	int Formulation_Day;
	double Target_Location;
	int Mission_Duration;
	type Mission_Type;
	int ID;
	int Significance;
	int Waiting_Time;
	int Completion_Day;
	int Execution_Days;
	MISSION_STATUS status;


public:
	Mission(int ID, int FD, type Type, double TL,int MD, int Sig);
	Mission();
	void setID(int ID);
	int getID();

	//Formulation_Day
	void setFD(int FD);
	int getFD();

	//Completion_Day
	void setCD(int CD);
	int getCD();

	//Execution_Day
	void setED(int ED);
	int getED();

	//Target_Location
	void setTL(double TL);
	int getTL();

	//Mission_Duration
	void setMD(int MD);
	int getMD();

	// Mission_Type
	void setMT(type T);
	type getMT();

	//Significance
	void setSIG(int Sig);
	int getSIG();

	//waiting days
	void set_Waiting_Time(int wait);
	int get_Waiting_Time();

	int calc_Execution_time(int speed);
	int calc_Completion_time(int speed);

	//Whether mission is waiting, in-execution or completed
	void SetStatus(MISSION_STATUS s);
	MISSION_STATUS  GetStatus() const;

};
#endif