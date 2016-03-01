#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "setting.h"
#include "macro.h"

extern simSpec gSpec;
extern std11 gStd;

static struct option options[] = {
	{"help", no_argument, NULL, 'h'},
	{"debug", no_argument, NULL, 'd'},
	{"fd", no_argument, NULL, 'f'},
	{"ofdma", no_argument, NULL, 'o'},
	{"std", required_argument, NULL, 's'},
	{"numSTA", required_argument, NULL, 'n'},
	{"simTime", required_argument, NULL, 't'},
	{"traffic", required_argument, NULL, 'l'},
	{"trial", required_argument, NULL, 'r'},
	{"lambdaSta", required_argument, NULL, 'm'},
	{"delay", required_argument, NULL, 'a'},
	{0, 0, 0, 0}
};

void simSetting(int argc, char **argv){
	int opt;
	int index;

	gSpec.fDebug = false;
	gSpec.simTime = 10;
	gSpec.fFd = false;
	gSpec.fOfdma = false;
	gSpec.trafficPattern = 0;
	gSpec.numTrial = 1;
	gSpec.lambdaSta = 0.1;
	gSpec.delayMode = 0;   //from arriving at buffer to transmitting

	printf("-----Settings-----\n");

	while((opt = getopt_long(argc, argv, "hdfos:n:t:l:r:m:a:", options, &index)) != -1){
		switch(opt){
			case 'h':
				printf(
					"   -h, --help: Show this help.\n"
					"   -d, --debug: Debug mode.\n"
					"   -f, --fd: Full-duplex mode.\n"
					"   -o, --ofdma: OFDMA mode.\n"
					"   -s, --std: Select standard from a/n/ac.\n"
					"   -n, --numSTA: Number of STAs.\n"
					"   -t, --simTime: Simulation time (sec)\n"
					"   -l, --traffic: Traffic Pattern.\n"
					"   -r, --trial: number of Simulation runs\n"
					"   -m, --lambdaSta: Lambda of STA (/us)\n"
					"   -a, --delay: delayMode (0/1)."
				);
				exit(1);
				break;
			case 'd':
				printf("   Debug mode.\n");
				gSpec.fDebug = true;
				break;
			case 'f':
				printf("   Full-duplex mode.\n");
				gSpec.fFd = true;
				break;
			case 'o':
				printf("   OFDMA mode.\n");
				gSpec.fOfdma = true;
				break;
			case 's':
				gStd.std = optarg;
				printf("   Standard is 11%s.\n", gStd.std);
				break;
			case 'n':
				gSpec.numSTA = atoi(optarg);
				printf("   Number of STA is %d.\n", gSpec.numSTA);
				break;
			case 't':
				gSpec.simTime = atoi(optarg);
				printf("   Simulation time is %d sec.\n", gSpec.simTime);
				break;
			case 'l':
				gSpec.trafficPattern = atoi(optarg);
				printf("   Traffic Pattern is %d.\n", gSpec.trafficPattern);
				break;
			case 'm':
				gSpec.lambdaSta = atof(optarg);
				printf("   Lambda of STA is %f /us.\n", gSpec.lambdaSta);
				break;
			case 'r':
				gSpec.numTrial = atoi(optarg);
				printf("   Simulation run %d times.\n", gSpec.numTrial);
				break;
			case 'a':
				gSpec.delayMode = atoi(optarg);
				printf("   Delay Mode is %d.\n", gSpec.delayMode);
				break;
			default:
				printf("Illegal options! \'%c\' \'%c\'\n", opt, optopt);
				exit(1);
		}
	}

	printf("------------------\n");

	if(gSpec.fOfdma==true && gSpec.numSTA<2){
		printf("Connot OFDMA mode.\n");
		exit(10);
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
	//gSpec.numTrial = 1;
	gSpec.lambdaAp = 0.1;
	//gSpec.lambdaSta = 0.1;
}
