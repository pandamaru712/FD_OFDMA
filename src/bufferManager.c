#include <stdio.h>
#include <stdlib.h>
#include "bufferManager.h"
#include "frameGen.h"
#include "macro.h"

extern double gElapsedTime;
extern simSpec gSpec;
extern FILE *gFileSta;

void swapAp(apInfo *ap){
	int i;
	int numNotZero = 0;
	frameInfo temp[BUFFER_SIZE] = {};

	for(i=0; i<BUFFER_SIZE; i++){
		if(ap->buffer[i].lengthMsdu!=0){
			if(i==0){
				printf("Error\n");
			}
			temp[numNotZero].lengthMsdu = ap->buffer[i].lengthMsdu;
			temp[numNotZero].timeStamp = ap->buffer[i].timeStamp;
			numNotZero++;
		}
	}
	for(i=0; i<numNotZero; i++){
		ap->buffer[i].lengthMsdu = temp[i].lengthMsdu;
		ap->buffer[i].timeStamp = temp[i].timeStamp;

		if(gSpec.delayMode==1){
			if(i==0){
				if(ap->buffer[0].timeStamp==0){
					ap->buffer[0].timeStamp = gElapsedTime;
				}else{
					printf("Time Stamp Error.");
					exit(40);
				}
			}
		}
	}
	for(; i<BUFFER_SIZE; i++){
		ap->buffer[i].lengthMsdu = 0;
		ap->buffer[i].timeStamp = 0;
	}
}

void swapSta(staInfo *sta){
	int i;
	int numNotZero = 0;
	frameInfo temp[BUFFER_SIZE] = {};

	for(i=0; i<BUFFER_SIZE; i++){
		if(sta->buffer[i].lengthMsdu!=0){
			if(i==0){
				printf("Error\n");
			}
			temp[numNotZero].lengthMsdu = sta->buffer[i].lengthMsdu;
			temp[numNotZero].timeStamp = sta->buffer[i].timeStamp;
			numNotZero++;
		}
	}
	for(i=0; i<numNotZero; i++){
		sta->buffer[i].lengthMsdu = temp[i].lengthMsdu;
		sta->buffer[i].timeStamp = temp[i].timeStamp;

		if(gSpec.delayMode==1){
			if(i==0){
				if(sta->buffer[0].timeStamp==0){
					sta->buffer[0].timeStamp = gElapsedTime;
				}else{
					printf("Time Stamp Error.");
					exit(40);
				}
			}
		}
	}
	for(; i<BUFFER_SIZE; i++){
		sta->buffer[i].lengthMsdu = 0;
		sta->buffer[i].timeStamp = 0;
	}
}

void arriveAp(apInfo *ap, double span){
	int i;
	bool fFirst = true;
	double timeSum = 0.0;

	for(i=0; i<BUFFER_SIZE; i++){
		if(ap->buffer[i].lengthMsdu==0){
			if(fFirst==true){
				if(ap->timeNextFrame>span){
					ap->timeNextFrame -= span;
					break;
				}else{
					ap->buffer[i].lengthMsdu = ap->waitFrameLength;
					timeSum += ap->timeNextFrame;
					span -= ap->timeNextFrame;
					ap->timeNextFrame = poisson(false);
					if(i==0 || gSpec.delayMode==0){
						ap->buffer[i].timeStamp = gElapsedTime + timeSum;
					}else{
						ap->buffer[i].timeStamp = 0;
					}
					ap->sumFrameLengthInBuffer += ap->buffer[i].lengthMsdu;
					fFirst = false;
				}
				//ap->waitFrameLength = traffic(false);
			}else{
				if(ap->timeNextFrame>span){
					ap->timeNextFrame -= span;
					ap->waitFrameLength = traffic(false);
					break;
				}else{
					ap->buffer[i].lengthMsdu = traffic(false);
					timeSum += ap->timeNextFrame;
					span -= ap->timeNextFrame;
					ap->timeNextFrame = poisson(false);
					if(gSpec.delayMode==0){
						ap->buffer[i].timeStamp = gElapsedTime + timeSum;
					}else if(gSpec.delayMode==1){
						ap->buffer[i].timeStamp = 0;
					}else{
						printf("Delay Mode Error.\n");
						exit(41);
					}
					ap->sumFrameLengthInBuffer += ap->buffer[i].lengthMsdu;
				}
			}
			if(ap->sumFrameLengthInBuffer>(gSpec.bufferSizeByte*1000)){
				ap->waitFrameLength = ap->buffer[i].lengthMsdu;
				ap->sumFrameLengthInBuffer -= ap->buffer[i].lengthMsdu;
				ap->buffer[i].lengthMsdu = 0;
				ap->buffer[i].timeStamp = 0.0;
				break;
			}
			if(i==BUFFER_SIZE){
				printf("BUFFER_SIZE is not enough.\n");
				ap->waitFrameLength = traffic(false);
			}
		}
	}
}

void arriveSta(staInfo *sta, double span){
	int i;
	bool fFirst = true;
	double timeSum = 0.0;

	for(i=0; i<BUFFER_SIZE; i++){
		if(sta->buffer[i].lengthMsdu==0){
			if(fFirst==true){
				if(sta->timeNextFrame>span){
					sta->timeNextFrame -= span;
					break;
				}else{
					sta->buffer[i].lengthMsdu = sta->waitFrameLength;
					timeSum += sta->timeNextFrame;
					span -= sta->timeNextFrame;
					sta->timeNextFrame = poisson(true);
					if(i==0 || gSpec.delayMode==0){
						sta->buffer[i].timeStamp = gElapsedTime + timeSum;
					}else{
						sta->buffer[i].timeStamp = 0;
					}
					sta->sumFrameLengthInBuffer += sta->buffer[i].lengthMsdu;
					fFirst = false;
				}
				//sta->waitFrameLength = traffic(true);
			}else{
				if(sta->timeNextFrame>span){
					sta->timeNextFrame -= span;
					sta->waitFrameLength = traffic(true);
					break;
				}else{
					sta->buffer[i].lengthMsdu = traffic(true);
					timeSum += sta->timeNextFrame;
					span -= sta->timeNextFrame;
					sta->timeNextFrame = poisson(true);
					if(gSpec.delayMode==0){
						sta->buffer[i].timeStamp = gElapsedTime + timeSum;
					}else if(gSpec.delayMode==1){
						sta->buffer[i].timeStamp = 0;
					}else{
						printf("Delay Mode Error.\n");
						exit(41);
					}
					sta->sumFrameLengthInBuffer += sta->buffer[i].lengthMsdu;
				}
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
	/*if(i==BUFFER_SIZE){
		printf("BUFFER_SIZE is not enough.\n");
		sta->waitFrameLength = traffic(true);
	}*/
	/*if(sta->buffer[BUFFER_SIZE-1].lengthMsdu!=0){
		printf("Limited2");
	}*/
}
