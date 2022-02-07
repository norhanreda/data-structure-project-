#include "UserInterface.h"

Sim_Mode UI::GetUIMode()
{
	Sim_Mode Mode;
	int choice;
	cout << "Please select UI mode : (1)Interactive, (2)StepByStep, (3)Silent " << endl;

	cin >> choice;
	while (choice < 1 || choice > 3)
	{
		cout << "Invalid Choice, please select from previous list" << endl;
		cin >> choice;
	}

	if (choice == 1)
	{
		Mode = Interactive;
	}
	else if (choice == 2)
	{
		Mode = StepByStep;
	}
	else if(choice == 3)
	{
		Mode = Silent;
	}
	
	return Mode;
}

string UI::GetFileName()
{
	string FileName;
	cout << "Enter file name: (HAS TO BE IN TEST CASES FILE)" << endl;
	cin >> FileName;

	return FileName;
}


void UI::PrintOnConsole(MarsStation* pStation)
{
	int CurrDay = pStation->getCurrentDay();
	Mission* M;
	Mission* temp_mission;
	rovers* R;
	rovers* temp_rover;
	int first = 0;																			//Checks if it's the first element to be printed
	int last;																				//Checks if it's the last element to be printed

	//Get needed lists from mars station class

	Queue<Mission*>* Waiting_Mountainous = pStation->get_Mountainous_Missions();			//Waiting emergency missions list
	Queue<Mission*>* Waiting_Polar = pStation->get_Polar_Missions();						//Waiting polar missions list
	PriorityQueue<Mission*>* Waiting_Emergency = pStation->get_Emergency_Missions();		//Waiting mountainous missions list

	PriorityQueue<Mission*>* tempE = new PriorityQueue<Mission*>;

	Queue<rovers*>* Emergency_Rovers = pStation->get_EmergencyRovers();						//Waiting emergency rovers list
	Queue<rovers*>* Mountainous_Rovers = pStation->get_MountainousRovers();					//Waiting polar rovers list
	Queue<rovers*>* Polar_Rovers = pStation->get_PolarRovers();								//Waiting mountainous rovers list

	int N_Complete = pStation->get_Comp_Missions_count();									//Number of completed missions
	Queue<Mission*>* Complete_Missions = pStation->get_Completed_Missions();				//Completed missions list	
																							
	Queue<Mission*>* tempM = new Queue<Mission*>;

	int N_AssignedR = pStation->get_Assigned_Rovers_count();								//Number of assigned rovers
	int N_CheckupR = pStation->get_InCheck_Rovers_count();									//Number of rovers in checkup
	PriorityQueue<rovers*>* Assigned_Rovers = pStation->get_AssignedRovers();				//Assigned rovers list
	PriorityQueue<rovers*>* Checkup_Rovers = pStation->get_CheckupRovers();					//Checkup rovers list

	PriorityQueue<rovers*>* tempR = new PriorityQueue<rovers*>;

	//*************************************************Printing on Console*************************************************


	cout << "Current Day:" << CurrDay << endl;
	cout << pStation->get_wait_Missions_count() << " Wating Missions: ";

	last = pStation->get_Emer_Missions_count();

	//Print waiting emergency missions
	for (int i = 0; i < pStation->get_Emer_Missions_count(); i++)
	{
		Waiting_Emergency->dequeue(M);
		if (first >= 1)
			cout << ',' << M->getID();
		else
		{
			cout <<"["<< M->getID();
			first++;
		}
		tempE->enqueue(M, pStation->CalcPriority(M));
		if (i + 1 == last)
			cout << ']';
	}

	while (!tempE->isEmpty())
	{
		tempE->dequeue(M);
		Waiting_Emergency->enqueue(M, pStation->CalcPriority(M));
	}

	last = pStation->get_Polar_Missions_count();

	//Print waiting polar missions
	for (int i = 0; i < pStation->get_Polar_Missions_count(); i++)
	{
		Waiting_Polar->dequeue(M);
		if (i >= 1)
			cout << ',' << M->getID();
		else
			cout << " (" << M->getID();
		Waiting_Polar->enqueue(M);
		if (i + 1 == last)
			cout << ')';
	}

	last = pStation->get_Mount_Missions_count();

	//Print waiting mountainous missions
	for (int i = 0; i < pStation->get_Mount_Missions_count(); i++)
	{
		Waiting_Mountainous->dequeue(M);
		if (i >= 1)
			cout << ',' << M->getID();
		else
			cout <<" {"<< M->getID();
		Waiting_Mountainous->enqueue(M);
		if (i + 1 == last)
			cout << '}';
	}

	cout << endl << "------------------------------------------------------------------------" << endl;

	//***********************************************In-Execution Missions and Rovers************************************************

	first = 0;

	//last = N_AssignedR;
	cout << N_AssignedR << " In-Execution Missions/Rovers: ";

	//Make pointer to the last emergency mission
	for (int i = 0; i < N_AssignedR && !Assigned_Rovers->isEmpty(); i++)
	{
		Assigned_Rovers->dequeue(R);
		if (R->getmission()->getMT() == Emergency)
		{
			M = R->getmission();
		}
		tempR->enqueue(R, - (R->getmission()->getCD()));
	}

	while (!tempR->isEmpty())
	{
		tempR->dequeue(R);
		Assigned_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	//Print in-execution emergency missions and assigned rovers
	for (int i = 0; i < N_AssignedR && !Assigned_Rovers->isEmpty(); i++)
	{
		Assigned_Rovers->dequeue(R);
		if (R->getmission()->getMT() == Emergency)
		{
			if (R->getmission() == M)
				if(first == 0)
				cout << " [" << R->getmission()->getID() << "/" << R->getroverID() << ']';
				else
				cout << ',' << R->getmission()->getID() << "/" << R->getroverID() << ']';
			else if(first >= 1)
				cout << ',' << R->getmission()->getID() << "/" << R->getroverID();
			else
			{
				cout <<" ["<< R->getmission()->getID() << "/" << R->getroverID();
				first++;
			}

		}
		tempR->enqueue(R, -(R->getmission()->getCD()));
	}

	first = 0;

	//Make pointer to the last polar mission
	for (int i = 0; i < N_AssignedR && !tempR->isEmpty(); i++)
	{
		tempR->dequeue(R);
		if (R->getmission()->getMT() == Polar)
		{
			M = R->getmission();
		}
		Assigned_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	while (!Assigned_Rovers->isEmpty())
	{
		Assigned_Rovers->dequeue(R);
		tempR->enqueue(R, -(R->getmission()->getCD()));
	}

	//Print in-execution polar missions and assigned rovers
	for (int i = 0; i < N_AssignedR && !tempR->isEmpty(); i++)
	{
		tempR->dequeue(R);
		if (R->getmission()->getMT() == Polar)
		{
			if (R->getmission() == M)
				if (first == 0)
					cout << " (" << R->getmission()->getID() << "/" << R->getroverID() << ')';
				else
					cout << ',' << R->getmission()->getID() << "/" << R->getroverID() << ')';
			else if (first >= 1)
				cout << ',' << R->getmission()->getID() << "/" << R->getroverID();
			else
			{
				cout << " (" << R->getmission()->getID() << "/" << R->getroverID();
				first++;
			}
		}
		Assigned_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	first = 0;

	//Make pointer to the last mountainous mission
	for (int i = 0; i < N_AssignedR && !Assigned_Rovers->isEmpty(); i++)
	{
		Assigned_Rovers->dequeue(R);
		if (R->getmission()->getMT() == Mountainous)
		{
			M = R->getmission();
		}
		tempR->enqueue(R, -(R->getmission()->getCD()));
	}

	while (!tempR->isEmpty())
	{
		tempR->dequeue(R);
		Assigned_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	//Print in-execution mountainous missions and assigned rovers
	for (int i = 0; i < N_AssignedR && !Assigned_Rovers->isEmpty(); i++)
	{
		Assigned_Rovers->dequeue(R);
		if (R->getmission()->getMT() == Mountainous)
		{
			if (R->getmission() == M)
				if (first == 0)
					cout << " {" << R->getmission()->getID() << "/" << R->getroverID() << '}';
				else
					cout << ',' << R->getmission()->getID() << "/" << R->getroverID() << '}';
			else if (first >= 1)
				cout << ',' << R->getmission()->getID() << "/" << R->getroverID();
			else
			{
				cout<<" {" << R->getmission()->getID() << "/" << R->getroverID();
				first++;
			}
		}
		tempR->enqueue(R, -(R->getmission()->getCD()));	
	}

	cout << endl << "------------------------------------------------------------------------" << endl;

	//Return all assigned rovers to original list
	while (!tempR->isEmpty())
	{
		tempR->dequeue(R);
		Assigned_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	//****************************************************Rovers Info****************************************************
	cout << pStation->get_Available_Rovers_count() << " Available Rovers: ";

	last = pStation->get_Emer_Rovers_count();

	// Print available emergency rovers
	for (int i = 0; i < pStation->get_Emer_Rovers_count(); i++)
	{
		Emergency_Rovers->dequeue(R);
		if (i >= 1)
			cout << ',' << R->getroverID();
		else
			cout <<"["<< R->getroverID();
		Emergency_Rovers->enqueue(R);
		if (i + 1 == last)
			cout << ']';
	}

	last = pStation->get_Polar_Rovers_count();

	//Print available polar rovers
	for (int i = 0; i < pStation->get_Polar_Rovers_count(); i++)
	{
		Polar_Rovers->dequeue(R);
		if (i >= 1)
			cout << ',' << R->getroverID();
		else
			cout <<" ("<< R->getroverID();
		Polar_Rovers->enqueue(R);
		if (i + 1 == last)
			cout << ')';
	}

	last = pStation->get_Mount_Rovers_count();

	//Print available mountainous rovers
	for (int i = 0; i < pStation->get_Mount_Rovers_count(); i++)
	{
		Mountainous_Rovers->dequeue(R);
		if (i >= 1)
			cout << ',' << R->getroverID();
		else
			cout << " {" << R->getroverID();
		Mountainous_Rovers->enqueue(R);
		if (i + 1 == last)
			cout << '}';
	}

	cout << endl << "------------------------------------------------------------------------" << endl;
	
	cout << pStation->get_InCheck_Rovers_count() << " In-Checkup Rovers: ";

	first = 0;
	
	//Make pointer to last in Check up emergency rover
	for (int i = 0; i < N_CheckupR && !Checkup_Rovers->isEmpty(); i++)
	{
		Checkup_Rovers->dequeue(R);
		if (R->getroverType() == Emergency)
		{
			temp_rover = R;
		}
		tempR->enqueue(R, -(R->get_CheckUp_end_Day()));
	}

	while (!tempR->isEmpty())
	{
		tempR->dequeue(R);
		Checkup_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	//Print in checkup emergency rovers
	for (int i = 0; i < N_CheckupR && !Checkup_Rovers->isEmpty(); i++)
	{
		Checkup_Rovers->dequeue(R);
		if (R->getroverType() == Emergency)
		{
			if(R == temp_rover)
				if (first == 0)
					cout << " [" << R->getroverID() << ']';
				else
					cout << ',' << R->getroverID() << ']';
			else if (first >= 1)
				cout << ',' << R->getroverID();
			else
			{
				cout <<" ["<< R->getroverID();
				first++;
			}
		}
		tempR->enqueue(R , -(R->get_CheckUp_end_Day()));
	}

	first = 0;

	//Make pointer to last in Check up polar rover
	for (int i = 0; i < N_CheckupR && !tempR->isEmpty(); i++)
	{
		tempR->dequeue(R);
		if (R->getroverType() == Polar)
		{
			temp_rover = R;
		}
		Checkup_Rovers->enqueue(R, -(R->get_CheckUp_end_Day()));
	}

	while (!Checkup_Rovers->isEmpty())
	{
		Checkup_Rovers->dequeue(R);
		tempR->enqueue(R, -(R->getmission()->getCD()));
	}

	//Print in checkup polar rovers
	for (int i = 0; i < N_CheckupR && !tempR->isEmpty(); i++)
	{
		tempR->dequeue(R);
		if (R->getroverType() == Polar)
		{
			if (R == temp_rover)
				if (first == 0)
					cout << " (" << R->getroverID() << ')';
				else
					cout << ',' << R->getroverID() << ')';
			else if (first >= 1)
				cout << ',' << R->getroverID();
			else
			{
				cout << " (" << R->getroverID();
				first++;
			}
		}
		Checkup_Rovers->enqueue(R, -(R->get_CheckUp_end_Day()));
		
	}

	first = 0;

	//Make pointer to last in Check up mountainous rover
	for (int i = 0; i < N_CheckupR && !Checkup_Rovers->isEmpty(); i++)
	{
		Checkup_Rovers->dequeue(R);
		if (R->getroverType() == Mountainous)
		{
			temp_rover = R;
		}
		tempR->enqueue(R, -(R->get_CheckUp_end_Day()));
	}

	while (!tempR->isEmpty())
	{
		tempR->dequeue(R);
		Checkup_Rovers->enqueue(R, -(R->getmission()->getCD()));
	}

	//Print in checkup mountainous rovers
	for (int i = 0; i < N_CheckupR && !Checkup_Rovers->isEmpty(); i++)
	{
		Checkup_Rovers->dequeue(R);
		if (R->getroverType() == Mountainous)
		{
			if (R == temp_rover)
				if (first == 0)
					cout << " {" << R->getroverID() << '}';
				else
					cout << ',' << R->getroverID() << '}';
			else if (first >= 1)
				cout << ',' << R->getroverID();
			else
			{
				cout << " {" << R->getroverID();
				first++;
			}

		}
		tempR->enqueue(R, -(R->get_CheckUp_end_Day()));	
	}

	cout << endl << "------------------------------------------------------------------------" << endl;

	//Return all rovers in checkup to original list
	while (!tempR->isEmpty())
	{
		tempR->dequeue(R);
		Checkup_Rovers->enqueue(R, -(R->get_CheckUp_end_Day()));
	}

	//***************************************************Completed Missions***************************************************
	cout << N_Complete << " Completed Missions: ";

	first = 0;

	//Make pointer to last completed emergency mission
	for (int i = 0; i < N_Complete && !Complete_Missions->isEmpty(); i++)
	{
		Complete_Missions->dequeue(M);
		if (M->getMT() == Emergency)
		{
			temp_mission = M;
		}
		tempM->enqueue(M);
	}
	while (!tempM->isEmpty())
	{
		tempM->dequeue(M);
		Complete_Missions->enqueue(M);
	}

	//Print completed emergency missions
	for (int i = 0; i < N_Complete && !Complete_Missions->isEmpty() ; i++)
	{
		Complete_Missions->dequeue(M);
		if (M->getMT() == Emergency)
		{
			if (M == temp_mission)
				if (first == 0)
					cout << " [" << M->getID() << ']';
				else
					cout << ',' << M->getID() << ']';
			else if (first >= 1)
				cout << ',' << M->getID();
			else
			{
				cout << "[" << M->getID();
				first++;
			}
		}
		tempM->enqueue(M);	
	}

	first = 0;

	//Make pointer to last completed polar mission
	for (int i = 0; i < N_Complete && !tempM->isEmpty(); i++)
	{
		tempM->dequeue(M);
		if (M->getMT() == Polar)
		{
			temp_mission = M;
		}
		Complete_Missions->enqueue(M);
	}
	while (!Complete_Missions->isEmpty())
	{
		Complete_Missions->dequeue(M);
		tempM->enqueue(M);
	}

	//Print completed polar missions
	for (int i = 0; i < N_Complete && !tempM->isEmpty(); i++)
	{
		tempM->dequeue(M);
		if (M->getMT() == Polar)
		{
			if (M == temp_mission)
				if (first == 0)
					cout << " (" << M->getID() << ')';
				else
					cout << ',' << M->getID() << ')';
			else if (first >= 1)
				cout << ',' << M->getID();
			else
			{
				cout << " (" << M->getID();
				first++;
			}
		}
		Complete_Missions->enqueue(M);	
	}

	first = 0;

	//Make pointer to last completed mountainous mission
	for (int i = 0; i < N_Complete && !Complete_Missions->isEmpty(); i++)
	{
		Complete_Missions->dequeue(M);
		if (M->getMT() == Mountainous)
		{
			temp_mission = M;
		}
		tempM->enqueue(M);
	}
	while (!tempM->isEmpty())
	{
		tempM->dequeue(M);
		Complete_Missions->enqueue(M);
	}

	//Print completed moutainous missions
	for (int i = 0; i < N_Complete && !Complete_Missions->isEmpty(); i++)
	{
		Complete_Missions->dequeue(M);
		if (M->getMT() == Mountainous)
		{
			if (M == temp_mission)
				if (first == 0)
					cout << " {" << M->getID() << '}';
				else
					cout << ',' << M->getID() << '}';
			else if (first >= 1)
				cout << ',' << M->getID();
			else
			{
				cout << " {" << M->getID();
				first++;
			}

		}
		tempM->enqueue(M);	
	}

	cout << endl << "------------------------------------------------------------------------" << endl;

	//Return all completed missions to original list
	while (!tempM->isEmpty())
	{
		tempM->dequeue(M);
		Complete_Missions->enqueue(M);
	}

	//Empty allocated memory
	delete tempR;
	delete tempM;
	delete tempE;
	tempR = nullptr;
	tempM = nullptr;
	tempE = nullptr;
}

void UI::PrintSilentMode()
{
	cout << "Silent Mode" << endl;
	cout << "Simulation Starts..." << endl;
	cout << "Simulation ends, Output file created" << endl;
}




