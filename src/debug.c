#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "setting.h"
#include "macro.h"

extern std11 gStd;
extern simSpec gSpec;
extern FILE *gFileSta;

void debug(void){
	printf("timeAck = %d\n", gStd.timeAck);
	printf("timeRts = %d\n", gStd.timeRts);
	printf("timeCts = %d\n", gStd.timeCts);
	printf("afterColl = %d\n", gStd.afterColl);
	printf("afterSucc = %d\n", gStd.afterSucc);
	printf("eifs = %d\n", gStd.eifs);
	printf("%d\n", RAND_MAX);
}

void debugSta(staInfo* sta, int nodeID){
	int i;
	long test=0;

	for(int i=0; i<BUFFER_SIZE; i++){
		test += sta->buffer[i].lengthMsdu;
	}
	//if(sta->buf!=test){
		//fprintf(gFileSta, "STA %d\n", nodeID);
		fprintf(gFileSta, "%ld\n", sta->sumFrameLengthInBuffer);
		for(i=0; i<BUFFER_SIZE; i++){
			if(sta->buffer[i].lengthMsdu!=0){
				fprintf(gFileSta, "%d-%d,", i, sta->buffer[i].lengthMsdu);
			}
		}
		if(i==BUFFER_SIZE){
			fprintf(gFileSta, "\n**********\n\n\n");
		}
	//}
}
