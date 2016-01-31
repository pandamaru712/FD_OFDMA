#ifndef _nodeInfo_h
#define _nodeInfo_h

#include "setting.h"

extern simSpec gSpec;

typedef struct frameInformation{
	int lengthMsdu;
	double timeStamp;
}frameInfo;

typedef struct staInformatioin{
	frameInfo buffer[200];
	int backoffCount;
	int cw;
	int retryCount;
	long numTxFrame;
	long numCollFrame;
	long numLostFrame;
	long numSuccFrame;
	long byteSuccFrame;
	bool fCollNow;
	int afterColl;
	bool fSuccNow;
	int afterSucc;
	bool fTx;
	long sumFrameLengthInBuffer;
	double sumDelay;
	int waitFrameLength;
}staInfo;

typedef struct apInformation{
	frameInfo buffer[200];
	int backoffCount;
	int cw;
	int retryCount;
	long numTxFrame;
	long numCollFrame;
	long numLostFrame;
	long numSuccFrame;
	long byteSuccFrame;
	bool fCollNow;
	int afterColl;
	bool fSuccNow;
	int afterSucc;
	bool fTx;
	long sumFrameLengthInBuffer;
	double sumDelay;
	int waitFrameLength;
}apInfo;

#endif
