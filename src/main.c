#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checkOption.h"

double gElapsedTime;

void simSetting(int, char**);

int main(int argc, char *argv[]){
	//Check option values from command line.
	checkOption(argc, argv);
	//Apply option values to simulation settings.
	simSetting(argc,argv);

	return 0;
}

void simSetting(int argc, char **argv);
