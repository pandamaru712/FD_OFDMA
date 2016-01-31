#include <stdio.h>
#include <stdlib.h>
#include "success.h"
#include "setting.h"
#include "bufferManager.h"

extern double gElapsedTime;
extern simSpec gSpec;
extern std11 gStd;

void txSuccess(staInfo sta[], apInfo *ap, int *numTx){
	int i;
	int txFrameLength;
	int txTimeFrameLength;
	int totalTime;

	if(ap->fTx==true){
		if(ap->buffer[0].lengthMsdu!=0){
			ap->retryCount = 0;
			ap->cw = gStd.cwMin;
			ap->backoffCount = rand() % (ap->cw + 1);
			ap->fSuccNow = true;
			ap->fTx = false;

			ap->sumFrameLengthInBuffer -= ap->buffer[0].lengthMsdu;
			ap->byteSuccFrame += ap->buffer[0].lengthMsdu;
			txFrameLength = ap->buffer[0].lengthMsdu;
			ap->buffer[0].lengthMsdu = 0;
			ap->sumDelay += (gElapsedTime - ap->buffer[0].timeStamp);
			ap->buffer[0].timeStamp = 0;
			ap->numSuccFrame++;
			ap->numTxFrame++;
			swapAp(ap);

			for(i=0; i<gSpec.numSTA; i++){
				if(sta[i].buffer[0].lengthMsdu!=0){
					if(sta[i].backoffCount>0){
						sta[i].backoffCount--;
					}else{
						printf("Really?\n");
					}
				}
				sta[i].fSuccNow = false;
			}
		}else{
			printf("txAP's buffer is empty.\n");
			exit(5);
		}
	}else{
		for(i=0; i<gSpec.numSTA; i++){
			if(sta[i].fTx==true){
				if(sta[i].buffer[0].lengthMsdu==0){
					printf("txSTA's buffer is empty.\n");
					exit(6);
				}
				sta[i].retryCount = 0;
				sta[i].cw = gStd.cwMin;
				sta[i].backoffCount = rand() % (sta[i].cw + 1);
				sta[i].fSuccNow = true;
				sta[i].fTx = false;

				sta[i].sumFrameLengthInBuffer -= sta[i].buffer[0].lengthMsdu;
				sta[i].byteSuccFrame += sta[i].buffer[0].lengthMsdu;
				txFrameLength = sta[i].buffer[0].lengthMsdu;
				sta[i].buffer[0].lengthMsdu = 0;
				sta[i].sumDelay += (gElapsedTime-sta[i].buffer[0].timeStamp);
				sta[i].buffer[0].timeStamp = 0;
				sta[i].numTxFrame++;
				sta[i].numSuccFrame++;
				swapSta(&sta[i]);
			}else{
				if(sta[i].buffer[0].lengthMsdu!=0){
					if(sta[i].backoffCount>0){
						sta[i].backoffCount--;
					}else{
						printf("Really?\n");
					}
				}
				sta[i].fSuccNow = false;
				if(ap->buffer[0].lengthMsdu!=0){
					if(ap->backoffCount>0){
						ap->backoffCount--;
					}else{
						printf("Really?\n");
					}
				}
				ap->fSuccNow = false;
			}
		}
	}

	txTimeFrameLength = gStd.phyHeader + 4 * ((gStd.macService + 8* (gStd.macHeader + txFrameLength + gStd.macFcs) + gStd.macTail + (4 * gStd.dataRate - 1)) / (4 * gStd.dataRate));
	totalTime = txTimeFrameLength + gStd.sifs + gStd.timeAck;


	for(i=0; i<gSpec.numSTA; i++){
		arriveSta(&sta[i], totalTime);
	}
	arriveAp(ap, totalTime);

	*numTx = 0;
	gElapsedTime += (double)totalTime;
}

void afterSucc(staInfo sta[], apInfo *ap, int *numTx){
	int minAfterSucc = 0;
	int i;
	*numTx = 0;

	for(i=0; i<gSpec.numSTA; i++){
		if(sta[i].buffer[0].lengthMsdu!=0){
			if(sta[i].fSuccNow==false){
				sta[i].afterSucc = sta[i].backoffCount * gStd.slot + gStd.afterSucc;
				minAfterSucc = sta[i].afterSucc;
			}else{
				sta[i].afterSucc = sta[i].backoffCount + gStd.slot + gStd.difs;
				minAfterSucc = sta[i].afterSucc;
			}
		}
	}
	if(ap->buffer[0].lengthMsdu!=0){
		if(ap->fSuccNow==false){
			ap->afterSucc = ap->backoffCount * gStd.slot + gStd.afterSucc;
			minAfterSucc = ap->afterSucc;
		}else{
			ap->afterSucc = ap->backoffCount * gStd.slot + gStd.afterSucc;
			minAfterSucc = ap->afterSucc;
		}
	}

	if(minAfterSucc==0){
		printf("All nodes' buffer is empty.\n");
	}

	for(i=0; i<gSpec.numSTA; i++){
		if((sta[i].afterSucc < minAfterSucc)&&(sta[i].buffer[0].lengthMsdu!=0)){
			minAfterSucc = sta[i].afterSucc;
		}
	}
	if((ap->afterSucc < minAfterSucc)&&(ap->buffer[0].lengthMsdu!=0)){
		minAfterSucc = ap->afterSucc;
	}
	for(i=0; i<gSpec.numSTA; i++){
      if(sta[i].buffer[0].lengthMsdu!=0){
      	if(sta[i].fSuccNow == false){
            if(minAfterSucc > gStd.afterSucc){
                sta[i].backoffCount -= ((minAfterSucc - gStd.afterSucc) / gStd.slot);
            }
         }else{
            if(minAfterSucc > gStd.difs){
               sta[i].backoffCount -= ((minAfterSucc - gStd.difs) / gStd.slot);
            }
         }
         if(sta[i].afterSucc == minAfterSucc){
            (*numTx)++;
            sta[i].fTx = true;
         }
      }
      sta[i].fSuccNow = false;
   }

   if(ap->buffer[0].lengthMsdu!=0){
      if(ap->fSuccNow == false){
         if(minAfterSucc > gStd.afterSucc){
            ap->backoffCount -= ((minAfterSucc - gStd.afterSucc) / gStd.slot);
         }
      }else{
         if(minAfterSucc > gStd.difs){
            ap->backoffCount -= ((minAfterSucc - gStd.difs) / gStd.slot);
         }
      }
      if(ap->afterSucc == minAfterSucc){
         (*numTx)++;
         ap->fTx = true;
   	}
   }
   ap->fSuccNow = false;

	for(i=0; i<gSpec.numSTA; i++){
      arriveSta(&sta[i], minAfterSucc);
   }
   arriveAp(ap, minAfterSucc);

   gElapsedTime += (double)minAfterSucc;
}
