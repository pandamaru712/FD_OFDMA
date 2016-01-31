#include <stdlib.h>
#include "initialization.h"
#include "setting.h"

extern std11 gStd;

void initializeResult(resultInfo *result){
	result->aveStaThroughput = 0.0;
	result->apThroughput = 0.0;
	result->aveThroughput = 0.0;
	result->aveStaProColl = 0.0;
	result->apProColl = 0.0;
	result->aveProColl = 0.0;
	result->aveDelay = 0.0;
}

void initializeNodeInfo(staInfo sta[], apInfo* ap){
	int i, j;

	for(i=0; i<gSpec.numSTA; i++){
		for(j=0; j<200; j++){
			sta[i].buffer[j].lengthMsdu = 0;
			sta[i].buffer[j].timeStamp = 0.0;
		}
		sta[i].backoffCount = rand() % (gStd.cwMin + 1);
		sta[i].retryCount = 0;
		sta[i].numTxFrame = 0;
		sta[i].numCollFrame = 0;
		sta[i].numLostFrame = 0;
		sta[i].numSuccFrame = 0;
		sta[i].byteSuccFrame = 0;
		sta[i].fCollNow = false;
		sta[i].afterColl = 0;
		sta[i].fSuccNow = false;
		sta[i].afterSucc = 0;
		sta[i].fTx = false;
		sta[i].sumFrameLengthInBuffer = 0;
		sta[i].sumDelay = 0.0;
	}

	for(i=0; i<200; i++){
		ap->buffer[i].lengthMsdu = 0;
		ap->buffer[i].timeStamp = 0.0;
	}
	ap->backoffCount = rand() % (gStd.cwMin + 1);
	ap->retryCount = 0;
	ap->numTxFrame = 0;
	ap->numCollFrame = 0;
	ap->numLostFrame = 0;
	ap->numSuccFrame = 0;
	ap->byteSuccFrame = 0;
	ap->fCollNow = false;
	ap->afterColl = 0;
	ap->fSuccNow = false;
	ap->afterSucc = 0;
	ap->fTx = false;
	ap->sumFrameLengthInBuffer = 0;
	ap->sumDelay = 0.0;
}
