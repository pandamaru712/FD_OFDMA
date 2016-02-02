#include <stdio.h>
#include "bufferManager.h"
#include "frameGen.h"

extern double gElapsedTime;
extern simSpec gSpec;

void swapAp(apInfo *ap){
	int i;
	int numNotZero = 0;
	frameInfo temp[200] = {};

	for(i=0; i<200; i++){
		if(ap->buffer[i].lengthMsdu!=0){
			temp[numNotZero].lengthMsdu = ap->buffer[i].lengthMsdu;
			temp[numNotZero].timeStamp = ap->buffer[i].timeStamp;
			numNotZero++;
		}
	}
	for(i=0; i<numNotZero; i++){
		ap->buffer[i].lengthMsdu = temp[i].lengthMsdu;
		ap->buffer[i].timeStamp = temp[i].timeStamp;
		if(i==0){
			if(ap->buffer[0].timeStamp==0){
				ap->buffer[0].timeStamp = gElapsedTime;
			}
		}
	}
	for(; i<200; i++){
		ap->buffer[i].lengthMsdu = 0;
		ap->buffer[i].timeStamp = 0;
	}
}

void swapSta(staInfo *sta){
	int i;
	int numNotZero = 0;
	frameInfo temp[200] = {};

	for(i=0; i<200; i++){
		if(sta->buffer[i].lengthMsdu!=0){
			temp[numNotZero].lengthMsdu = sta->buffer[i].lengthMsdu;
			temp[numNotZero].timeStamp = sta->buffer[i].timeStamp;
			numNotZero++;
		}
	}
	for(i=0; i<numNotZero; i++){
		sta->buffer[i].lengthMsdu = temp[i].lengthMsdu;
		sta->buffer[i].timeStamp = temp[i].timeStamp;
		if(i==0){
			if(sta->buffer[0].timeStamp==0){
				sta->buffer[0].timeStamp = gElapsedTime;
			}
		}
	}
	for(; i<200; i++){
		sta->buffer[i].lengthMsdu = 0;
		sta->buffer[i].timeStamp = 0;
	}
}

void arriveAp(apInfo *ap, int span){
	int i;
	bool fFirst = true;
	double timeSum = 0.0;

	for(i=0; i<200; i++){
		if(ap->buffer[i].lengthMsdu==0){
			if(fFirst==true){
				timeSum = poisson(false);
				if(timeSum>span){
					break;
				}
				ap->buffer[i].lengthMsdu = ap->waitFrameLength;
				ap->buffer[i].timeStamp = 0;   //gElapsedTime + timeSum;
				ap->sumFrameLengthInBuffer += ap->buffer[i].lengthMsdu;
				fFirst = false;
				ap->waitFrameLength = traffic(false);
			}else{
				timeSum += poisson(false);
				if(timeSum>span){
					break;
				}
				ap->buffer[i].lengthMsdu = traffic(false);
				ap->buffer[i].timeStamp = 0;   //gElapsedTime + timeSum;
				ap->sumFrameLengthInBuffer += ap->buffer[i].lengthMsdu;
			}
			if(ap->sumFrameLengthInBuffer>(gSpec.bufferSizeByte*1000)){
				ap->waitFrameLength = ap->buffer[i].lengthMsdu;
				ap->sumFrameLengthInBuffer -= ap->buffer[i].lengthMsdu;
				ap->buffer[i].lengthMsdu = 0;
				ap->buffer[i].timeStamp = 0.0;
				break;
			}
		}
	}
}

void arriveSta(staInfo *sta, int span){
	int i;
	bool fFirst = true;
	double timeSum = 0.0;

	for(i=0; i<200; i++){
		if(sta->buffer[i].lengthMsdu==0){
			if(fFirst==true){
				timeSum = poisson(true);
				if(timeSum>span){
					break;
				}
				sta->buffer[i].lengthMsdu = sta->waitFrameLength;
				sta->buffer[i].timeStamp = 0;   //gElapsedTime + timeSum;
				sta->sumFrameLengthInBuffer += sta->buffer[i].lengthMsdu;
				fFirst = false;
				sta->waitFrameLength = traffic(true);
			}else{
				timeSum += poisson(false);
				if(timeSum>span){
					break;
				}
				sta->buffer[i].lengthMsdu = traffic(true);
				sta->buffer[i].timeStamp = 0;   //gElapsedTime + timeSum;
				sta->sumFrameLengthInBuffer += sta->buffer[i].lengthMsdu;
			}
			if(sta->sumFrameLengthInBuffer>(gSpec.bufferSizeByte*1000)){
				sta->waitFrameLength = sta->buffer[i].lengthMsdu;
				sta->sumFrameLengthInBuffer -= sta->buffer[i].lengthMsdu;
				sta->buffer[i].lengthMsdu = 0;
				sta->buffer[i].timeStamp = 0.0;
				break;
			}
		}
	}
}
