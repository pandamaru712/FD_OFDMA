#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "frameGen.h"

extern simSpec gSpec;

double poisson(bool isSta){
	double t, r;
	if(isSta==false){
		r = (double)rand() / RAND_MAX;
		t = (-1.0 / gSpec.lambdaAp) * (log(1.0-r));
		if(t<0){
			printf("Function 'frmaeGen' error.\n");
			exit(3);
		}
	}else{
		r = (double)rand() / RAND_MAX;
		t = (-1.0/gSpec.lambdaSta) * (log(1.0-r));
		if(t<0){
			printf("Function 'frmaeGen' error.\n");
			exit(3);
		}
	}

	return t;
}

int traffic(bool isSta){
	int lengthMsdu;

	switch(gSpec.trafficPattern){
		case 0:
			lengthMsdu = 1500;
			break;
		default:
			printf("Traffic pattern error.");
			exit(4);
			break;
	}

	return lengthMsdu;
}
