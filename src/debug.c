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
	if(sta->buffer[0].timeStamp==0){
		fprintf(gFileSta, "STA %d\n", nodeID);
	for(i=0; i<BUFFER_SIZE; i++){
		fprintf(gFileSta, "%d-%f,", sta->buffer[i].lengthMsdu, sta->buffer[i].timeStamp);
		if(i==199){
			fprintf(gFileSta, "\n**********\n\n\n");
		}
	}
}
}
