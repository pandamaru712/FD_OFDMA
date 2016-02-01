#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "setting.h"

extern std11 gStd;
extern simSpec gSpec;

void debug(void){
	printf("timeAck = %d\n", gStd.timeAck);
	printf("timeRts = %d\n", gStd.timeRts);
	printf("timeCts = %d\n", gStd.timeCts);
	printf("afterColl = %d\n", gStd.afterColl);
	printf("afterSucc = %d\n", gStd.afterSucc);
	printf("eifs = %d\n", gStd.eifs);
	printf("%d\n", RAND_MAX);
}
