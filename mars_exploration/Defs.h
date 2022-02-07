#ifndef __DEFS_H_
#define __DEFS_H_

enum type
{
	Mountainous,
	Polar,
	Emergency
};

enum Sim_Mode
{
	Interactive,
	StepByStep,
	Silent
};

enum MISSION_STATUS	// status
{
	Waiting,		//waiting 
	In_Execution,	//In-excaution but not complited  yet
	Completed, 	//compleated

};

#endif
