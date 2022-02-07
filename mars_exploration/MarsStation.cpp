#include"MarsStation.h"

#include<iostream>
using namespace std;


    MarsStation::MarsStation()
	 {
		Current_Day = 1;

		//missions lists
		Emergency_Missions = new PriorityQueue<Mission*>;	
		Polar_Missions = new Queue<Mission*>;					
		Mountainous_Missions = new Queue<Mission*>;	

		InExecution_Missions = new  PriorityQueue<Mission*>;
		Completed_Missions = new  Queue<Mission*>;

		//rovers lists
		EmergencyRovers = new Queue<rovers*>;		
		MountainousRovers = new Queue<rovers*>;
		PolarRovers = new Queue<rovers*>;

		AssignedRovers = new PriorityQueue<rovers*>;
		CheckupRovers = new PriorityQueue<rovers*>;


		//Events
		Events = new Queue<Event*>;

		AutoP_count = 0;				//Number of promoted missions
		N_Promoted_Missions = 0;

		// Numbers of rovers in each list
		Emer_Rovers_count = 0;			//Number of emergency rovers
		Mount_Rovers_count = 0;			//Number of mounatainous rovers
		Polar_Rovers_count = 0;			//Number of polar rovers
		Assigned_Rovers_count = 0;		//Number of assigned rovers
		InCheck_Rovers_count = 0;		//Number of rovers in checkup 
		total_Rovers_count = 0;			//Total number of rovers

		//Number of missions in each list
		Emer_Missions_count = 0;        // Number of emergency missions
		Mount_Missions_count = 0;		// Number of mountainous missions
		Polar_Missions_count = 0;		// Number of polar missions
		InExec_Missions_count = 0;		// Number of in execution missions
		Comp_Missions_count = 0;		// Number of completed missions
		total_Missions_count = 0;		// Total number of missions
	 }


	//Calls function to run the entire program
	void MarsStation::Simulate()
	{
		pUI = new UI;
		Sim_Mode UIMode = pUI->GetUIMode();
		
		if (UIMode == Interactive)
		{
			InteractiveMode();
		}
		else if (UIMode == StepByStep )
		{
			StepByStepMode();
		}
		else if (UIMode == Silent)
		{
			SilentMode();
		}
	}

	//*******************************************Events functions**********************************************

	//Adds mission to appropriate list
	void MarsStation:: AddToQueue(Mission* M, int prio)
	{
		if (M->getMT() == Emergency)
		{
			Emergency_Missions->enqueue(M, prio);
			Emer_Missions_count++;
		}
		else if (M->getMT() == Mountainous)
		{
			Mountainous_Missions->enqueue(M);
			Mount_Missions_count++;
		}
		else if (M->getMT() == Polar)
		{
			Polar_Missions->enqueue(M);
			Polar_Missions_count++;
		}
	}

	//Priority equation for emergency missions
	int MarsStation::CalcPriority(Mission * M)
	{
		return 1000*(M->getSIG())*(M->getFD())/(M->getTL())*(M->getMD());
	}

	//Executes events of the current day
	void MarsStation::ExecuteEvents()
	{
		Event* pEvent;
		while (Events->peekFront(pEvent))
		{
			if (pEvent->getED() <= Current_Day)
			{
				pEvent->Execute(this);
				Events->dequeue(pEvent);
				delete pEvent;
				pEvent = nullptr;
			}
			else
			{
				return;
			}
		}	
	}

	//Cancells mountainous mission if it's not executed
	void MarsStation::CancelMountainousMission(int Mission_ID)
	{
		Mission* temp;
		Queue<Mission*>* X = new Queue<Mission*>;
		int count = Mount_Missions_count;

		for (int i = 0; i < count && !Mountainous_Missions->isEmpty(); i++)
		{
			Mountainous_Missions->dequeue(temp);
			if (Mission_ID == temp->getID())
			{
				delete temp;
				temp = NULL;
				Mount_Missions_count--;
			}
			else
			{
				X->enqueue(temp);
			}
		}

		while(!X->isEmpty())
		{
			X->dequeue(temp);
			Mountainous_Missions->enqueue(temp);
		}
		delete X;
		X = NULL;
	}

	//Promotes mountainous missions to emergency missions (Event)
	void MarsStation::PromoteMountainousMission(int Mission_ID)
	{
		Mission* temp;
		Queue<Mission*>* X = new Queue<Mission*>;
		int count = Mount_Missions_count;

		for (int i = 0; i < count && !Mountainous_Missions->isEmpty(); i++)
		{
			Mountainous_Missions->dequeue(temp);
			if (Mission_ID == temp->getID())
			{
				temp->setMT(Emergency);
				int Priority = CalcPriority(temp);
				Emergency_Missions->enqueue(temp, Priority);
				Mount_Missions_count--;
				Emer_Missions_count++;
				N_Promoted_Missions++;
			}
			else
			{
				X->enqueue(temp);
			}
		}

		while(!X->isEmpty())
		{
			X->dequeue(temp);
			Mountainous_Missions->enqueue(temp);
		}
		delete X;
		X = NULL;
	}

	//Automatically promotes mountainous missions to emergency whose waiting days exceeded the AutoP
	void MarsStation::AutoPromote()
	{
		Mission* temp;
		Queue<Mission*>* X = new Queue<Mission*>;
		int count = Mount_Missions_count;
		
		for (int i = 0; i < count && !Mountainous_Missions->isEmpty(); i++)
		{
			Mountainous_Missions->dequeue(temp);
			if (temp->get_Waiting_Time() == AutoPromotion)		
			{
				temp->setMT(Emergency);
				int Priority = CalcPriority(temp);
				Emergency_Missions->enqueue(temp, Priority);
				Mount_Missions_count--;
				Emer_Missions_count++;
				AutoP_count++;
			}
			else
			{
				X->enqueue(temp);
			}
		}

		while(!X->isEmpty())
		{
			X->dequeue(temp);
			Mountainous_Missions->enqueue(temp);
		}
		delete X;
		X = NULL;
	}

	//Update waiting time of each mission in the waiting list if not assigned to rover on that day
	void MarsStation::UpdateWaitingTime()
	{
		PriorityQueue<Mission*>* temp = new PriorityQueue<Mission*>;
		Mission* M;

		for (int i = 0; i < Polar_Missions_count; i++)
		{
			Polar_Missions->dequeue(M);
			M->set_Waiting_Time(M->get_Waiting_Time() + 1);
			Polar_Missions->enqueue(M);
		}
		for (int i = 0; i < Mount_Missions_count; i++)
		{
			Mountainous_Missions->dequeue(M);
			M->set_Waiting_Time(M->get_Waiting_Time() + 1);
			Mountainous_Missions->enqueue(M);
		}

		for (int i = 0; i < Emer_Missions_count; i++)
		{
			Emergency_Missions->dequeue(M);
			M->set_Waiting_Time(M->get_Waiting_Time() + 1);
			temp->enqueue(M, CalcPriority(M));
		}

		while (!temp->isEmpty())
		{
			temp->dequeue(M);
			Emergency_Missions->enqueue(M, CalcPriority(M));
		}

		delete temp;
		temp = NULL;
	}

	// Assign available rovers to waiting missions each day
	void MarsStation::checkMission()
	{
		//Assign emergency missions to any available rover
		while (!Emergency_Missions->isEmpty())
		{
			Mission* emg;
			rovers* erg;

			//If there are emergency rovers
			if (!EmergencyRovers->isEmpty())
			{
				//Remove mission from waiting list
				Emergency_Missions->dequeue(emg);
				Emer_Missions_count--;

				//Remove rover from available list
				EmergencyRovers->dequeue(erg);
				Emer_Rovers_count--;

				//Move mission to in-execution 
				emg->setED(emg->calc_Execution_time(erg->getroverSpeed()));			//Calculate and set mission's execution days
				emg->setCD(emg->calc_Completion_time(erg->getroverSpeed()));		//Calculate and set mission's completion day		
				InExecution_Missions->enqueue(emg, -(emg->getCD()));
				InExec_Missions_count++;

				//Move rover to assigned and set its mission
				erg->setmission(emg);
				erg->set_IsAssigned(true);
				erg->set_IsAvailable(false);
				AssignedRovers->enqueue(erg, -(emg->getCD()));
				Assigned_Rovers_count++;
			}

			//If there are no emergency rovers and there are available mountainous rovers
			else if (!MountainousRovers->isEmpty())
			{
				//Remove mission from waiting list
				Emergency_Missions->dequeue(emg);
				Emer_Missions_count--;

				//Remove rover from available list
				MountainousRovers->dequeue(erg);
				Mount_Rovers_count--;

				//Move mission to in-execution 
				emg->setED(emg->calc_Execution_time(erg->getroverSpeed()));			//Calculate and set mission's execution days
				emg->setCD(emg->calc_Completion_time(erg->getroverSpeed()));		//Calculate and set mission's completion day
				InExecution_Missions->enqueue(emg, -(emg->getCD()));
				InExec_Missions_count++;

				//Move rover to assigned and set its mission
				erg->setmission(emg);
				erg->set_IsAssigned(true);
				erg->set_IsAvailable(false);
				AssignedRovers->enqueue(erg, -(emg->getCD()));
				Assigned_Rovers_count++;
			}

			//If there are no emergency or mountainous rovers and there are available polar rovers
			else if (!PolarRovers->isEmpty())
			{
				//Remove mission from waiting list
				Emergency_Missions->dequeue(emg);
				Emer_Missions_count--;

				//Remove rover from available list
				PolarRovers->dequeue(erg);
				Polar_Rovers_count--;

				//Move mission to in-execution 
				emg->setED(emg->calc_Execution_time(erg->getroverSpeed()));			//Calculate and set mission's execution days
				emg->setCD(emg->calc_Completion_time(erg->getroverSpeed()));		//Calculate and set mission's completion day
				InExecution_Missions->enqueue(emg, -(emg->getCD()));
				InExec_Missions_count++;

				//Move rover to assigned and set its mission
				erg->setmission(emg);
				erg->set_IsAssigned(true);
				erg->set_IsAvailable(false);
				AssignedRovers->enqueue(erg, -(emg->getCD()));
				Assigned_Rovers_count++;
			}
			else
				break;
		}

		//Assign polar mission to polar rover only
		while (!Polar_Missions->isEmpty())
		{
			Mission* plr;
			rovers* prov;
			if (!PolarRovers->isEmpty())
			{
				//Remove mission from waiting list
				Polar_Missions->dequeue(plr);
				Polar_Missions_count--;

				//Remove rover from available list
				PolarRovers->dequeue(prov);
				Polar_Rovers_count--;

				//Move mission to in-execution 
				plr->setED(plr->calc_Execution_time(prov->getroverSpeed()));		//Calculate and set mission's execution days
				plr->setCD(plr->calc_Completion_time(prov->getroverSpeed()));		//Calculate and set mission's completion day
				InExecution_Missions->enqueue(plr, -(plr->getCD()));
				InExec_Missions_count++;

				//Move rover to assigned list and set its mission
				prov->setmission(plr);
				prov->set_IsAssigned(true);
				prov->set_IsAvailable(false);
				AssignedRovers->enqueue(prov, -(plr->getCD()));
				Assigned_Rovers_count++;

			}
			else
				break;
		}

		//Assign mountainous missions to either mountainous or emergency rovers
		while (!Mountainous_Missions->isEmpty())
		{
			Mission* mount;
			rovers* mrov;

			//If there are mountainous rovers available
			if (!MountainousRovers->isEmpty())
			{
				//Remove mission from waiting list
				Mountainous_Missions->dequeue(mount);
				Mount_Missions_count--;

				//Remove rover from available list
				MountainousRovers->dequeue(mrov);
				Mount_Rovers_count--;

				//Move mission to in-execution 
				mount->setED(mount->calc_Execution_time(mrov->getroverSpeed()));		//Calculate and set mission's execution days
				mount->setCD(mount->calc_Completion_time(mrov->getroverSpeed()));		//Calculate and set mission's completion day
				InExecution_Missions->enqueue(mount, -(mount->getCD()));
				InExec_Missions_count++;

				//Move rover to assigned list and set its mission
				mrov->setmission(mount);
				mrov->set_IsAssigned(true);
				mrov->set_IsAvailable(false);
				AssignedRovers->enqueue(mrov, -(mount->getCD()));
				Assigned_Rovers_count++;

			}

			//If there are no mountainous rovers and there are emergency rovers available
			else if (!EmergencyRovers->isEmpty())
			{
				//Remove mission from waiting list
				Mountainous_Missions->dequeue(mount);
				Mount_Missions_count--;

				//Remove rover from available list
				EmergencyRovers->dequeue(mrov);
				Emer_Rovers_count--;

				//Move mission to in-execution 
				mount->setED(mount->calc_Execution_time(mrov->getroverSpeed()));		//Calculate and set mission's execution days
				mount->setCD(mount->calc_Completion_time(mrov->getroverSpeed()));		//Calculate and set mission's completion day
				InExecution_Missions->enqueue(mount, -(mount->getCD()));
				InExec_Missions_count++;

				//Move rover to assigned list and set its mission
				mrov->setmission(mount);
				mrov->set_IsAssigned(true);
				mrov->set_IsAvailable(false);
				AssignedRovers->enqueue(mrov, -(mount->getCD()));
				Assigned_Rovers_count++;
			}
			else
				break;
		}
	}

	//Checks assigned rovers and in-execution missions if completed and move them to completed list
	void MarsStation::Move_To_Completed()
	{
		bool end = true;
		Mission* mission_completed;
		rovers* rover_complete;

		while (AssignedRovers->peekFront(rover_complete) && end == true)
		{
			if ((rover_complete->getmission())->getCD() == Current_Day)
			{
				AssignedRovers->dequeue(rover_complete);
				Assigned_Rovers_count--;
				rover_complete->set_IsAssigned(false);

				//Move from in-execution to completed

				InExecution_Missions->dequeue(mission_completed);
				InExec_Missions_count--;
				Completed_Missions->enqueue(mission_completed);
				mission_completed->SetStatus(Completed);
				Comp_Missions_count++;


				if (rover_complete->Is_Need_Check_Up())		//Move rover to checkup
				{
					rover_complete->set_CheckUp_end_Day(rover_complete->getcheckupDuration() + Current_Day);
					CheckupRovers->enqueue(rover_complete, -(rover_complete->get_CheckUp_end_Day()));
					rover_complete->set_IsUnderCheckUP(true);
					InCheck_Rovers_count++;
				}
				else		//Return to available of its type
				{
					type rover_type;
					rover_type = rover_complete->getroverType();
					if (rover_type == Emergency)
					{
						EmergencyRovers->enqueue(rover_complete);
						Emer_Rovers_count++;
						rover_complete->set_IsAvailable(true);
					}
					else if (rover_type == Mountainous)
					{
						MountainousRovers->enqueue(rover_complete);
						Mount_Rovers_count++;
						rover_complete->set_IsAvailable(true);
					}
					else if (rover_type == Polar)
					{
						PolarRovers->enqueue(rover_complete);
						Polar_Rovers_count++;
						rover_complete->set_IsAvailable(true);
					}
				}

			}
			else
				end = false;
		}
	}

	//Return rovers from checkup to available 
	void MarsStation::move_CheckUp_rovers()
	{
		bool end = true;

		rovers* rover_checked;
		while (!CheckupRovers->isEmpty() && end == true)
		{
			CheckupRovers->peekFront(rover_checked);
			if (rover_checked->get_CheckUp_end_Day() == Current_Day)
			{
				CheckupRovers->dequeue(rover_checked);
				InCheck_Rovers_count--;
				type rover_type;
				rover_type = rover_checked->getroverType();
				if (rover_type == Emergency)
				{
					EmergencyRovers->enqueue(rover_checked);
					Emer_Rovers_count++;
				}
				else if (rover_type == Mountainous)
				{
					MountainousRovers->enqueue(rover_checked);
					Mount_Rovers_count++;
				}
				else if (rover_type == Polar)
				{
					PolarRovers->enqueue(rover_checked);
					Polar_Rovers_count++;
				}
				rover_checked->set_IsUnderCheckUP(false);
				rover_checked->set_IsAvailable(true);
			}
			else
				end = false;
		}
	}

	//***************************Dealing with input and output files******************************
	void MarsStation::ReadInputFile()
	{
		ifstream inputFile;
		string FileName;
		FileName = pUI->GetFileName();

		FileName = "Test Cases\\" + FileName + ".txt";

		inputFile.open(FileName);
		if (!inputFile.is_open())
		{
			cout << "Can't access file!";
			return;
		}
		//Read number of rovers of each type
		int M_Rovers, P_Rovers, E_Rovers;
		inputFile >> M_Rovers >> P_Rovers >> E_Rovers;

		//Read speed of rovers of each type
		int SM, SP, SE;
		inputFile >> SM >> SP >> SE;

		//Checkup info
		int N, CM, CP, CE;
		inputFile >> N >> CM >> CP >> CE;

		//Place rovers in appropriate lists
		for (int i = 0; i < M_Rovers; i++)
		{
			rovers* R = new rovers( i +1 , Mountainous, SM, N ,CM);
			MountainousRovers->enqueue(R);
		}
		Mount_Rovers_count = M_Rovers;		//Assign number of mountainous rovers		

		for (int i = 0; i < P_Rovers; i++)
		{
			rovers* R = new rovers((i + 1)+ M_Rovers, Polar, SP, N, CP);
			PolarRovers->enqueue(R);
		}
		Polar_Rovers_count = P_Rovers;		//Assign number of polar rovers

		for (int i = 0; i < E_Rovers; i++)
		{
			rovers* R = new rovers((i + 1)+ M_Rovers + P_Rovers, Emergency, SE, N, CE);
			EmergencyRovers->enqueue(R);
		}
		Emer_Rovers_count = E_Rovers;		//Assign number of emergency rovers

		total_Rovers_count = E_Rovers + M_Rovers + P_Rovers;

		//Number of waiting days before mountainous is automatically promoted
		int AutoP;				
		inputFile >> AutoP;
		setAutoP(AutoP);

		//Read number of events
		int E;
		inputFile >> E;

		char Event_Type, T;
		type TYP;
		int ED, ID, MDUR, SIG;
		double  TLOC;
		for (int i = 0; i < E; i++)
		{
			inputFile >> Event_Type;
			if (Event_Type == 'F')			//Create new formulation event
			{
				inputFile >> T >> ED >> ID >> TLOC >> MDUR >> SIG;
				//Check mission's type
				if (T == 'M')		
				{
					TYP = (type)Mountainous;
				}
				else if (T == 'P')
				{
					TYP = (type)Polar;
				}
				else if (T == 'E')
				{
					TYP = (type)Emergency;
				}

				Event* Formulation = new  FormulationEvent(TYP, ED, ID, TLOC, MDUR, SIG);		
				Events->enqueue(Formulation);
			}
			else if (Event_Type == 'X')			//Create new cancellation event
			{
				inputFile >> ED >> ID;
				Event* Cancellation = new CancellationEvent(ED, ID);		
				Events->enqueue(Cancellation);
			}
			else if (Event_Type == 'P')		//Create new promotion event
			{
				inputFile >> ED >> ID;
				Event* Promotion = new PromotionEvent(ED, ID);				
				Events->enqueue(Promotion);
			}
		}
	}

	void MarsStation::WriteOutputFile()
	{
		ofstream outputFile;
		Mission* M;
		double total_Wait_Time = 0;
		double total_Execution_Time = 0;
		outputFile.open("Output Files\\Output.txt");

		outputFile << "CD" << "\t" << "ID" << "\t" << "FD" << " \t" << "WD" << " \t" << "ED" << endl;
		for (int i = 0; i < Comp_Missions_count; i++)
		{
			Completed_Missions->dequeue(M);
			//print CD
			outputFile << M->getCD() << "\t" << M->getID() << "\t" << M->getFD() << "\t" << M->get_Waiting_Time() << "\t" << M->getED() << endl;
			total_Wait_Time += M->get_Waiting_Time();
			total_Execution_Time += M->getED();

			if (M->getMT() == Emergency)
				Emer_Missions_count++;
			else if (M->getMT() == Mountainous)
				Mount_Missions_count++;
			else if (M->getMT() == Polar)
				Polar_Missions_count++;
		}
		outputFile << "………………………………………………………………………………………………" << endl << "………………………………………………………………………………………………" << endl;
		outputFile << "Missions: " << Comp_Missions_count << "[M: " << Mount_Missions_count << ", P: " << Polar_Missions_count << ", E: " << Emer_Missions_count << "]" << endl;
		outputFile << "Rovers: " << total_Rovers_count << "[M: " << Mount_Rovers_count << ", P: " << Polar_Rovers_count << ", E: " << Emer_Rovers_count << "]" << endl;
		outputFile << "Avg Wait = " << total_Wait_Time / Comp_Missions_count;
		outputFile << ", Avg Exec = " << total_Execution_Time / Comp_Missions_count << endl;
		if (Mount_Missions_count + AutoP_count + N_Promoted_Missions == 0)
		{
			outputFile << "Auto-promoted: " << 0 << '%' << endl;
		}
		else
			outputFile << "Auto-promoted: " << (AutoP_count / (Mount_Missions_count + AutoP_count + N_Promoted_Missions)) * 100 << '%' << endl;
		
		outputFile.close();

	}

	//*******************************************User Interface Modes*******************************************
	void MarsStation::InteractiveMode()
	{
		ReadInputFile();
		while (!( Mountainous_Missions->isEmpty() && Polar_Missions->isEmpty()
			     && Emergency_Missions->isEmpty() && Events->isEmpty() 
			     && InExecution_Missions->isEmpty() && CheckupRovers->isEmpty()))
		{
			//Execute  all functions for the day 
			UpdateWaitingTime();
			ExecuteEvents();
			AutoPromote();
			Move_To_Completed();
			move_CheckUp_rovers();
			checkMission();
			
			pUI->PrintOnConsole(this);
			Current_Day++;
			system("pause");			//Waits for user input
		}
		WriteOutputFile();
	}

	void MarsStation::StepByStepMode()
	{
		ReadInputFile();
		while (!(Mountainous_Missions->isEmpty() && Polar_Missions->isEmpty()
			&& Emergency_Missions->isEmpty() && Events->isEmpty()
			&& InExecution_Missions->isEmpty() && CheckupRovers->isEmpty()))
		{
			//Execute  all functions for the day
			UpdateWaitingTime();
			ExecuteEvents();
			AutoPromote();
			Move_To_Completed();
			move_CheckUp_rovers();
			checkMission();

			pUI->PrintOnConsole(this);
			Current_Day++;
			_sleep(1000);			//Waits for one second
		}
		WriteOutputFile();
	}

	void MarsStation::SilentMode()
	{
		ReadInputFile();
		while (!(Mountainous_Missions->isEmpty() && Polar_Missions->isEmpty()
			&& Emergency_Missions->isEmpty() && Events->isEmpty()
			&& InExecution_Missions->isEmpty() && CheckupRovers->isEmpty()))
		{
			//Execute  all functions for the day
			UpdateWaitingTime();
			ExecuteEvents();
			AutoPromote();
			Move_To_Completed();
			move_CheckUp_rovers();
			checkMission();
			Current_Day++;
		}
		pUI->PrintSilentMode();
		WriteOutputFile();
	}

	//******************************************Setters and Getters******************************************//
	void MarsStation::setAutoP(int AutoP)
	{
		AutoPromotion = AutoP;
	}

	int MarsStation::getCurrentDay()
	{
		return Current_Day;
	}
	int MarsStation::get_Emer_Rovers_count()
	{
		return Emer_Rovers_count;
	}

	int MarsStation::get_Mount_Rovers_count()
	{
		return Mount_Rovers_count;
	}

	int MarsStation::get_Polar_Rovers_count()
	{
		return Polar_Rovers_count;
	}

	int MarsStation::get_Available_Rovers_count()
	{
		return (Emer_Rovers_count + Mount_Rovers_count + Polar_Rovers_count);
	}

	int MarsStation::get_Assigned_Rovers_count()
	{
		return Assigned_Rovers_count;
	}

	int MarsStation::get_InCheck_Rovers_count()
	{
		return InCheck_Rovers_count;
	}

	int MarsStation::get_Emer_Missions_count()
	{
		return Emer_Missions_count;
	}

	int MarsStation::get_Mount_Missions_count()
	{
		return Mount_Missions_count;
	}

	int MarsStation::get_Polar_Missions_count()
	{
		return Polar_Missions_count;
	}


	int MarsStation::get_wait_Missions_count()
	{
		return (Polar_Missions_count + Mount_Missions_count + Emer_Missions_count);
	}

	int MarsStation::get_Comp_Missions_count()
	{
		return Comp_Missions_count;
	}

	PriorityQueue<Mission*>* MarsStation::get_Emergency_Missions()
	{
		return Emergency_Missions;
	}

	Queue<Mission*>* MarsStation::get_Polar_Missions()
	{
		return Polar_Missions;
	}

	Queue<Mission*>* MarsStation::get_Mountainous_Missions()
	{
		return Mountainous_Missions;
	}

	Queue<Mission*>* MarsStation::get_Completed_Missions()
	{
		return Completed_Missions;
	}

	Queue<rovers*>* MarsStation::get_EmergencyRovers() const
	{
		return EmergencyRovers;
	}

	Queue<rovers*>* MarsStation::get_MountainousRovers() const
	{
		return MountainousRovers;
	}

	Queue<rovers*>* MarsStation::get_PolarRovers() const
	{
		return PolarRovers;
	}

	PriorityQueue<rovers*>* MarsStation::get_AssignedRovers()
	{
		return AssignedRovers;
	}

	PriorityQueue<rovers*>* MarsStation::get_CheckupRovers()
	{
		return CheckupRovers;
	}

	//Emsah kol hagaaa
	MarsStation::~MarsStation()
	{
		if (pUI)
			delete pUI;

		delete Events;

		delete Emergency_Missions;
		delete Polar_Missions;
		delete Mountainous_Missions;
		
		delete EmergencyRovers;
		delete MountainousRovers;;
		delete PolarRovers;

		delete AssignedRovers;
		delete CheckupRovers;

		delete InExecution_Missions;
		delete Completed_Missions;	
	}
