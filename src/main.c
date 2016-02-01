#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "nodeInfo.h"
#include "setting.h"
#include "initialization.h"
#include "success.h"
#include "collision.h"
#include "idle.h"
#include "result.h"
#include "debug.h"

double gElapsedTime;
std11 gStd;
simSpec gSpec;

static struct option options[] = {
	{"help", no_argument, NULL, 'h'},
	{"debug", no_argument, NULL, 'd'},
	{"std", required_argument, NULL, 's'},
	{"numSTA", required_argument, NULL, 'n'},
	{"simTime", required_argument, NULL, 't'},
	{0, 0, 0, 0}
};

void simSetting(int, char**);

int main(int argc, char *argv[]){
	//Check option values from command line.
	//checkOption(argc, argv);
	//Apply option values to simulation settings.
	simSetting(argc,argv);

	staInfo *sta;
	sta = (staInfo*)malloc(sizeof(staInfo)*gSpec.numSTA);
	apInfo ap;
	resultInfo result;
	//Intialize result information.
	initializeResult(&result);

	if(gSpec.fDebug==true){
		debug();
		exit(99);
	}

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
				txSuccess(sta, &ap, &numTx);
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
				//Wrong
				afterSuccess(sta, &ap, &numTx);
			}else{
				txCollision(sta, &ap);
				afterCollision(sta, &ap, &numTx);
			}
		}
		simulationResult(sta, &ap, &result, trialID);
	}

	free(sta);
	return 0;
}

void simSetting(int argc, char **argv){
	int opt;
	int index;

	gSpec.fDebug = false;
	gSpec.simTime = 10;

	while((opt = getopt_long(argc, argv, "hs:n:", options, &index)) != -1){
		switch(opt){
			case 'h':
				printf(
					"-h, --help: Show this help.\n"
					"-d, --debug: Debug mode.\n"
					"-s, --std: Select standard from a/n/ac.\n"
					"-n, --numSTA: Number of STAs.\n"
					"-t, --simTime: Simulation time (sec)\n"
				);
				exit(1);
				break;
			case 'd':
				printf("Debug mode.\n");
				gSpec.fDebug = true;
				break;
			case 's':
				gStd.std = optarg;
				printf("%s\n", gStd.std);
				break;
			case 'n':
				gSpec.numSTA = atoi(optarg);
				printf("Number of STA is %d.\n", gSpec.numSTA);
				break;
			case 't':
				gSpec.simTime = atoi(optarg);
				break;
			default:
				printf("Illegal options! \'%c\' \'%c\'\n", opt, optopt);
				exit(1);
		}
	}

	gStd.dataRate = 54;
	gStd.ackRate = 24;
	gStd.rtsRate = 54;
	gStd.ctsRate = 24;
	gStd.ackLength = 10;
	gStd.rtsLength = 16;
	gStd.ctsLength = 10;
	gStd.phyHeader = 20;
	gStd.macService = 16;
	gStd.macHeader = 24;
	gStd.macFcs = 4;
	gStd.macTail = 6;
	gStd.timeAck = gStd.phyHeader + 4 * ((gStd.macService + 8 * (gStd.ackLength + gStd.macFcs) + gStd.macTail + (4 * gStd.ackRate - 1)) / (4 * gStd.ackRate));
	gStd.timeRts = gStd.phyHeader + 4 * ((gStd.macService + 8 * (gStd.rtsLength + gStd.macFcs) + gStd.macTail + (4 * gStd.rtsRate - 1)) / (4 * gStd.rtsRate));
	gStd.timeCts = gStd.phyHeader + 4 * ((gStd.macService + 8 * (gStd.ctsLength + gStd.macFcs) + gStd.macTail + (4 * gStd.ctsRate - 1)) / (4 * gStd.ctsRate));
	gStd.sifs = 16;
	gStd.difs = 34;
	gStd.eifs = gStd.sifs + gStd.timeAck + gStd.difs;
	gStd.slot = 9;
	gStd.afterColl = gStd.difs;
	gStd.afterSucc = gStd.difs;
	gStd.ackTimeout = gStd.sifs + gStd.timeAck + gStd.slot;
	gStd.ctsTimeout = gStd.sifs + gStd.timeCts + gStd.slot;
	gStd.retryLimit = 6;
	gStd.cwMin = 15;
	gStd.cwMax = 1023;

	//gSpec.numSTA = 1;
	//gSpec.simTime = 10;
	gSpec.bufferSizeByte = 200;
	gSpec.numTrial = 1;
	gSpec.trafficPattern = 0;
	gSpec.lambdaAp = 0.1;
	gSpec.lambdaSta = 0.1;
}
