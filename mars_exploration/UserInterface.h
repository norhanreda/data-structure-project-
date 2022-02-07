#ifndef __USERINTERFACE_H_
#define __USERINTERFACE_H_

#pragma once
#include "UserInterface.h"
#include "MarsStation.h"
#include <iostream>

using namespace std;

class MarsStation;

class UI
{

public:
	Sim_Mode GetUIMode();
	void PrintOnConsole(MarsStation* pStation);
	void PrintSilentMode();
	string GetFileName();
};

#endif

