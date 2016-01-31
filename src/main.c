#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "checkOption.h"
#include "nodeInfo.h"
#include "setting.h"
#include "initialization.h"
#include "success.h"
#include "collision.h"
#include "idle.h"
#include "result.h"

double gElapsedTime;
std11 gStd;
simSpec gSpec;

static struct option options[] = {
	{"help", no_argument, NULL, 'h'},
	{"std", required_argument, NULL, 's'},
	{"numSTA", required_argument, NULL, 'n'},
	{0, 0, 0, 0}
};

void simSetting(int, char**);

int main(int argc, char *argv[]){
	//Check option values from command line.
	checkOption(argc, argv);
	//Apply option values to simulation settings.
	simSetting(argc,argv);

	staInfo *sta;
	sta = (staInfo*)malloc(sizeof(staInfo)*gSpec.numSTA);
	apInfo ap;
	resultInfo result;
	//Intialize result information.
	initializeResult(&result);

	int numTx = 0;
	int trialID;
	bool fEmpty = false;

	for (trialID=0; trialID<gSpec.numTrial; trialID++){
		srand(trialID);
		numTx = 0;
		fEmpty = true;
		initializeNodeInfo(sta, &ap);

		gElapsedTime += (double)gStd.difs;
		idle(sta, &ap, &numTx, &fEmpty);

		for(; gElapsedTime<gSpec.simTime*1000000;){
			if(numTx==1){
				txSuccess();
				fEmpty = true;
				for(int i=0; i<gSpec.numSTA; i++){
					if(sta[i].buffer[0].lengthMsdu!=0){
						fEmpty = false;
						break;
					}
				}
				if(ap.buffer[0].lengthMsdu!=0){
					fEmpty = false;
				}
				if(fEmpty==true){
					idle(sta, &ap, &numTx, &fEmpty);
				}
				afterSuccess();
			}else{
				txCollision();
				afterCollision();
			}
		}
		simulationResult();
	}

	free(sta);
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
