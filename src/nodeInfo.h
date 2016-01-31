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
}staInfo;

typedef struct apInformation{
	frameInfo buffer[200];
}apInfo;

#endif
