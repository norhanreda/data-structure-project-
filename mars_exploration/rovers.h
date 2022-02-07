#pragma once
#include"Defs.h"
#include"Mission.h"
class rovers
{
	int roverID;
	type roverType;
	int checkupDuration;
	int N_beforecheckup;      //no. of missions before checkup
	int roverSpeed;
	int CheckUp_end_Day;
	Mission* missionOfrover;
	int Executed_missions;
	bool isAvailable;
	bool isAssigned;
	bool isUnderCheckup;



public:

	rovers();
	rovers(int ID, type Type, int speed , int Mission_Checkup , int Checkup_Duration);
	~rovers();

	void setroverID(int id);
	int getroverID();


	void setroverType(type type);
	type getroverType();


	void setroverSpeed(int speed);
	int getroverSpeed();


	void setcheckupDuration(int duration);
	int getcheckupDuration();


	void setmission(Mission* m);
	Mission* getmission();


	///////////////
	void set_IsAvailable(bool isAva);
	bool get_IsAvailable();

	void set_IsAssigned(bool isAssig);
	bool get_IsAssigned();

	void set_IsUnderCheckUP(bool ischeck);
	bool get_IsUnderCheckUP();

	bool Is_Need_Check_Up();

	void set_CheckUp_end_Day(int endDay);
	int get_CheckUp_end_Day();

	//Number of days before the rover needs checkup
	void setN(int n);
	int getN();
};