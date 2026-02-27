#include "menu.h"


void callbackStartLearning(void)
{
	// the process wasn't started, exit the program with an error message...
	exit_loop = 1;
}


void callbackNewNetwork(void)
{
	exit_loop = 1;
}


/*
level_up - flag indicating should the cursor
		   go up one level regarding the menu level
		   Active on 1
*/
void callbackLevelUp(void)
{
	level_up = 1;
}


/*
exit_loop - if set true, this flag indicates the program should 
			be terminated
			Active on 1
*/
void callbackExit(void)
{
	exit_loop = 1;
}