#ifndef _setting_h
#define _setting_h

#include <unistd.h>
#include <getopt.h>

typedef enum boolean{
	false,
	true
}bool;

typedef struct standard{
	char *std;
	int dataRate;
	int ackRate;
	int rtsRate;
	int ctsRate;
	int timeAck;
	int timeRts;
	int timeCts;
	int sifs;
	int difs;
	int eifs;
	int slot;
	int afterColl;
	int afterSucc;
	int ackTimeout;
	int ctsTimeout;
	int retryLimit;
	int cwMin;
	int cwMax;
	int aMsduMax;
	int aMsduInAMpduMax;
	int aMpduMax;
	int phyHeader;   //us
	int macService;   //bits
	int macHeader;   //bytes
	int macFcs;   //bytes
	int macTail;   //bytes
}std11;

typedef struct specification{
	int numSTA;
	long simTime;   //us
	int bufferSize;
	int bufferSizeByte;   //kB
	int numTrial;
	int trafficPattern;
	double lambdaAp;
	double lambdaSta;

}simSpec;

#endif
