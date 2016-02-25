#include <stdlib.h>
#include "initialization.h"
#include "setting.h"
#include "frameGen.h"

extern std11 gStd;

void initializeResult(resultInfo *result){
	result->aveStaThroughput = 0.0;
	result->apThroughput = 0.0;
	result->aveThroughput = 0.0;
	result->aveStaProColl = 0.0;
	result->apProColl = 0.0;
	result->aveProColl = 0.0;
	result->aveStaDelay = 0.0;
	result->apDelay = 0.0;
	result->aveDelay = 0.0;
}

void initializeNodeInfo(staInfo sta[], apInfo* ap){
	int i, j;

	for(i=0; i<gSpec.numSTA; i++){
		for(j=0; j<200; j++){
			sta[i].buffer[j].lengthMsdu = 0;
			sta[i].buffer[j].timeStamp = 0.0;
			/*if(sta[i].sumFrameLengthInBuffer>(gSpec.bufferSizeByte*1000)){
				sta[i].waitFrameLength = sta[i].buffer[i].lengthMsdu;
				sta[i].sumFrameLengthInBuffer -= sta[i].buffer[i].lengthMsdu;
				sta[i].buffer[i].lengthMsdu = 0;
				sta[i].buffer[i].timeStamp = 0.0;
				break;
			}*/
		}
		sta[i].buffer[0].lengthMsdu = traffic(true);
		sta[i].sumFrameLengthInBuffer += sta[i].buffer[0].lengthMsdu;
		sta[i].waitFrameLength = traffic(true);
		sta[i].backoffCount = rand() % (gStd.cwMin + 1);
		sta[i].retryCount = 0;
		sta[i].numTxFrame = 0;
		sta[i].numCollFrame = 0;
		sta[i].numLostFrame = 0;
		sta[i].numSuccFrame = 0;
		sta[i].numPrimFrame = 0;
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
		ap->buffer[i].lengthMsdu = traffic(false);
		ap->buffer[i].timeStamp = 0.0;
		/*if(ap->sumFrameLengthInBuffer>(gSpec.bufferSizeByte*1000)){
			ap->waitFrameLength = ap->buffer[i].lengthMsdu;
			ap->sumFrameLengthInBuffer -= ap->buffer[i].lengthMsdu;
			ap->buffer[i].lengthMsdu = 0;
			ap->buffer[i].timeStamp = 0.0;
			break;
		}*/
	}
	ap->buffer[0].lengthMsdu = traffic(false);
	ap->sumFrameLengthInBuffer += ap->buffer[0].lengthMsdu;
	ap->waitFrameLength = traffic(false);
	ap->backoffCount = rand() % (gStd.cwMin + 1);
	ap->retryCount = 0;
	ap->numTxFrame = 0;
	ap->numCollFrame = 0;
	ap->numLostFrame = 0;
	ap->numSuccFrame = 0;
	ap->numPrimFrame = 0;
	ap->byteSuccFrame = 0;
	ap->fCollNow = false;
	ap->afterColl = 0;
	ap->fSuccNow = false;
	ap->afterSucc = 0;
	ap->fTx = false;
	ap->sumFrameLengthInBuffer = 0;
	ap->sumDelay = 0.0;
}
