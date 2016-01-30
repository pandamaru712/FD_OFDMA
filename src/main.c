#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "checkOption.h"
#include "nodeInfo.h"
#include "setting.h"

double gElapsedTime;
std11 gStd;
simSpec gSpec;

void simSetting(int, char**);

int main(int argc, char *argv[]){
	//Check option values from command line.
	checkOption(argc, argv);
	//Apply option values to simulation settings.
	simSetting(argc,argv);

	int numTx = 0;
	int trialID;
	bool flagEmpty = false;

	for (trialID=0; trialID<gSpec.numTrial; trialID++){
		srand(trialID);

		initializeValue(&numTx);

		gElapsedTime += (double)gStd.difs;
	}

	return 0;
}

void simSetting(int argc, char **argv){
	int opt;
	int index;

	while((opt = getopt_long(argc, argv, "hs:n:", options, &index)) != -1){
		switch(opt){
			switch (opt){
				case 'h':
					printf(
						"-h, --help: Show this help.\n"
						"-s, --std: Select standard from a/n/ac.\n"
						"-n, --numSTA: Number of STAs.\n"
					);
					break;
				case 's':
					gStd.std = optarg;
					printf("%s\n", gStd.std);
					break;
				case 'n':
					gSpec.numSTA = atoi(optarg);
					printf("%d\n", gSpec.numSTA);
					break;
				default:
					printf("Illegal options! \'%c\' \'%c\'\n", opt, optopt);
			}
		}
	}

}
