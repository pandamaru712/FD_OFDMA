#include <stdio.h>
#include <stdlib.h>
#include "result.h"

extern double gElapsedTime;
extern simSpec gSpec;

void simulationResult(staInfo sta[], apInfo *ap, resultInfo *result, int trialID){
	int i;

	long rNumFrameTx = 0;
	long rNumFrameSucc = 0;
	long rNumFrameColl = 0;
	long rNumFrameLost = 0;
	long rByteFrameSucc = 0;
	long rNumPrimFrame = 0;
	double rDelay = 0.0;

	for(i=0; i<gSpec.numSTA; i++){
		rNumFrameTx += sta[i].numTxFrame;
		rNumFrameSucc += sta[i].numSuccFrame;
		rNumFrameColl += sta[i].numCollFrame;
		rNumFrameLost += sta[i].numLostFrame;
		rNumPrimFrame += sta[i].numPrimFrame;
		rByteFrameSucc += sta[i].byteSuccFrame;
		rDelay += sta[i].sumDelay;
	}

	if(rNumFrameTx!=(rNumFrameSucc+rNumFrameColl)){
		printf("Somthing is wrong.\n");
	}
	if(ap->numTxFrame!=(ap->numSuccFrame+ap->numCollFrame)){
		printf("Somthing is wrong.\n");
	}

	//printf("%f\n", (double)rByteFrameSucc * 8 / gElapsedTime / gSpec.numSTA);

	result->aveStaThroughput += (double)rByteFrameSucc * 8 / gElapsedTime / gSpec.numSTA;
	result->apThroughput += (double)ap->byteSuccFrame * 8 / gElapsedTime;
	result->aveThroughput += (double)(rByteFrameSucc + ap->byteSuccFrame) * 8 /gElapsedTime;

	result->aveStaProColl += (double)rNumFrameColl / rNumPrimFrame;
	result->apProColl += (double)ap->numCollFrame / ap->numPrimFrame;
	result->aveProColl += (double)(rNumFrameColl + ap->numCollFrame) / (rNumPrimFrame + ap->numPrimFrame);

	result->aveStaDelay += (double)rDelay / rNumFrameSucc;
	result->apDelay += (double)ap->sumDelay / ap->numSuccFrame;
	result->aveDelay += (double)(rDelay + ap->sumDelay) / (rNumFrameSucc + ap->numSuccFrame);

	if(trialID==(gSpec.numTrial-1)){
		printf("STA1台あたりのスループットは%f Mbit/s\n", result->aveStaThroughput / gSpec.numTrial);
		printf("APのスループットは%f Mbit/s\n", result->apThroughput / gSpec.numTrial);
		printf("システムスループットは%f Mbit/s\n", result->aveThroughput / gSpec.numTrial);
		printf("STAの平均衝突率は%f \n", result->aveStaProColl / gSpec.numTrial);
		printf("APの衝突率は%f \n", result->apProColl / gSpec.numTrial);
		printf("システムの平均衝突率は%f \n", result->aveProColl / gSpec.numTrial);
		printf("STAの平均遅延は%f us\n", result->aveStaDelay / gSpec.numTrial);
		printf("APの遅延は%f us\n", result->apDelay / gSpec.numTrial);
		printf("システムの平均遅延は%f us\n", result->aveDelay / gSpec.numTrial);
	}
}
