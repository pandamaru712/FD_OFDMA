#ifndef _setting_h
#define _setting_h

typedef enum boolean{
	false,
	true
}bool;

typedef struct standard{
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
	int ackTimeout;
	int ctsTimeout;
	int retryLimit;
	int cwMin;
	int cwMax;
	int aMsduMax;
	int aMsduInAMpduMax;
	int aMpduMax;
}11Std;

typedef struct specification{
	int numSTA;
	long simTIme;   //us
	int bufferSize;
	int bufferSizeByte;
	int numTrial;
	int trafficPattern;
	double lambdaAp;
	double lambdaSta;

}simSpec;

#endif
