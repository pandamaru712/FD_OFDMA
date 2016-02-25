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
FILE *gFileSta;


void simSetting(int, char**);

int main(int argc, char *argv[]){
	//Check option values from command line.
	//checkOption(argc, argv);
	//Apply option values to simulation settings.
	simSetting(argc,argv);

	if((gFileSta=fopen("sta's buffer.txt", "w"))==NULL){
		printf("File cannot open! 3");
		exit(33);
	}

	staInfo *sta;
	sta = (staInfo*)malloc(sizeof(staInfo)*gSpec.numSTA);
	apInfo ap;
	resultInfo result;
	//Intialize result information.
	initializeResult(&result);

	if(gSpec.fDebug==true){
		debug();
		//printf("End debug.\n");
		//exit(99);
	}

	int numTx = 0;
	int trialID;
	bool fEmpty = false;

	for (trialID=0; trialID<gSpec.numTrial; trialID++){
		srand(trialID);
		numTx = 0;
		fEmpty = false;
		initializeNodeInfo(sta, &ap);

		gElapsedTime = (double)gStd.difs;
		idle(sta, &ap, &numTx, &fEmpty);

		for(; gElapsedTime<gSpec.simTime*1000000;){
			if(numTx==1){
				//debugSta(&sta[7],7);
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
				}else{
					afterSuccess(sta, &ap, &numTx);
				}
			}else{
				txCollision(sta, &ap);
				afterCollision(sta, &ap, &numTx);
			}
		}
		simulationResult(sta, &ap, &result, trialID);
	}

	fclose(gFileSta);
	free(sta);
	return 0;
}
