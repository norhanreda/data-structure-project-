#ifndef __MARSSTATION_H_
#define __MARSSTATION_H_

#include <fstream>
using namespace std;

#include"../mars_exploration/Queue.h"
#include"../mars_exploration/PriorityQueue.h"
#include"../mars_exploration/Mission.h"
#include"../mars_exploration/rovers.h"
#include "../mars_exploration/Event.h"
#include "../mars_exploration/FormulationEvent.h"
#include "../mars_exploration/CancellationEvent.h"
#include "../mars_exploration/PromotionEvent.h"
#include "../mars_exploration/Defs.h"
#include "../mars_exploration/UserInterface.h"
#include <cmath>


class Mission;
class rovers;
class Event;
class UI;

class MarsStation
{
	
private:

	//Missions
	PriorityQueue<Mission*>* Emergency_Missions;			//Waiting emergency missions list
	Queue<Mission*>* Polar_Missions;						//Waiting polar missions list
	Queue<Mission*>* Mountainous_Missions;					//Waiting mountainous missions list

	PriorityQueue<Mission*>* InExecution_Missions;			//In-Execution missions list
	Queue<Mission*>* Completed_Missions;					//Completed missions list

	//Rovers
	Queue<rovers*>* EmergencyRovers;						//Available emergency rovers list
	Queue<rovers*>* MountainousRovers;						//Available mountainous rovers list
    Queue<rovers*>* PolarRovers;							//Available polar rovers list

	PriorityQueue<rovers*> * AssignedRovers;				//Assigned rovers list
	PriorityQueue<rovers*> * CheckupRovers;					//Rovers in checkup list


	Queue<Event*> * Events;									//Event list 				

	UI *pUI;												//Pointer to User Interface to call mode functions 	
	int Current_Day;				
	int AutoPromotion;										//Number of days before mission is promoted
	double AutoP_count;										//Number of auto promoted missions
	int N_Promoted_Missions;
			
	//Numbers of rovers in each list
	int Emer_Rovers_count;
	int Mount_Rovers_count;
	int Polar_Rovers_count;
	int Assigned_Rovers_count;
	int InCheck_Rovers_count;
	int total_Rovers_count;

	//Number of missions in each list
	int Emer_Missions_count;
	int Mount_Missions_count;
	int Polar_Missions_count;
	int InExec_Missions_count;
	int Comp_Missions_count;
	int total_Missions_count;


public:
	MarsStation();
    ~MarsStation();

	void Simulate();

	void AddToQueue(Mission* M, int prio = 0);
	int CalcPriority(Mission * M);

	//Setters and getters
	void setAutoP(int AutoP);

	int getCurrentDay();
	int get_Emer_Rovers_count();
	int get_Mount_Rovers_count();
	int get_Polar_Rovers_count();
	int get_Available_Rovers_count(); 
	int get_Assigned_Rovers_count();
	int get_InCheck_Rovers_count();

	int get_Emer_Missions_count();
	int get_Mount_Missions_count();
	int get_Polar_Missions_count();
	int get_wait_Missions_count();  
	int get_Comp_Missions_count();

	PriorityQueue<Mission*>* get_Emergency_Missions();
	Queue<Mission*>* get_Polar_Missions();
	Queue<Mission*>* get_Mountainous_Missions();
	Queue<Mission*>* get_Completed_Missions();

	Queue<rovers*>* get_EmergencyRovers() const;
	Queue<rovers*>* get_MountainousRovers() const;
	Queue<rovers*>* get_PolarRovers() const;
	PriorityQueue<rovers*>* get_AssignedRovers();
	PriorityQueue<rovers*>* get_CheckupRovers();

	//Missions and Rovers
	void checkMission();			//Assigns mission to available rover
	void UpdateWaitingTime();			
	void Move_To_Completed();		//Move completed mission to completed list
	void move_CheckUp_rovers();		//Moves rover to checkup list

	//Events functions
	void ExecuteEvents();								//Execute events of each day
	void CancelMountainousMission(int Mission_ID);			
	void PromoteMountainousMission(int Mission_ID);		//Promotes mountainous missions to emergency 
	void AutoPromote();

	//Dealing with files
	void ReadInputFile();			//Loads data from input file
	void WriteOutputFile();			//Prints data to output file

	//UI modes
	void InteractiveMode();	
	void StepByStepMode();	
	void SilentMode();		



};
#endif