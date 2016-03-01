#ifndef _nodeInfo_h
#define _nodeInfo_h

#include "setting.h"
#include "macro.h"

extern simSpec gSpec;

typedef struct frameInformation{
	int lengthMsdu;
	double timeStamp;
}frameInfo;

typedef struct staInformatioin{
	frameInfo buffer[BUFFER_SIZE];
	int backoffCount;
	int cw;
	int retryCount;
	long numTxFrame;
	long numCollFrame;
	long numLostFrame;
	long numSuccFrame;
	long numPrimFrame;
	long byteSuccFrame;
	bool fCollNow;
	int afterColl;
	bool fSuccNow;
	int afterSucc;
	bool fTx;
	long sumFrameLengthInBuffer;
	double sumDelay;
	int waitFrameLength;
	double x;
	double y;
	double txPower;
	double antennaGain;
}staInfo;

typedef struct apInformation{
	frameInfo buffer[BUFFER_SIZE];
	int backoffCount;
	int cw;
	int retryCount;
	long numTxFrame;
	long numCollFrame;
	long numLostFrame;
	long numSuccFrame;
	long numPrimFrame;
	long byteSuccFrame;
	bool fCollNow;
	int afterColl;
	bool fSuccNow;
	int afterSucc;
	bool fTx;
	long sumFrameLengthInBuffer;
	double sumDelay;
	int waitFrameLength;
	double x;
	double y;
	double txPower;
	double antennaGain;
}apInfo;

#endif
