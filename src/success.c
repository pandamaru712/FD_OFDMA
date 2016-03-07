#include <stdio.h>
#include <stdlib.h>
#include "success.h"
#include "setting.h"
#include "bufferManager.h"
#include "debug.h"

extern double gElapsedTime;
extern simSpec gSpec;
extern std11 gStd;

void txSuccess(staInfo sta[], apInfo *ap, int *numTx){
	int i, j=0;
	int txFrameLength = 0;
	int txTimeFrameLength;
	int totalTime;
	int pairSta = gSpec.numSTA;
	int pairSta2nd = gSpec.numSTA;

	if(ap->fTx==true){
		if(ap->buffer[0].lengthMsdu!=0){
			ap->retryCount = 0;
			ap->cw = gStd.cwMin;
			ap->backoffCount = rand() % (ap->cw + 1);
			ap->fSuccNow = true;
			ap->fTx = false;

			ap->sumFrameLengthInBuffer -= ap->buffer[0].lengthMsdu;
			ap->byteSuccFrame += ap->buffer[0].lengthMsdu;
			if(txFrameLength<ap->buffer[0].lengthMsdu){
				txFrameLength = ap->buffer[0].lengthMsdu;
			}
			ap->buffer[0].lengthMsdu = 0;
			ap->sumDelay += (gElapsedTime - ap->buffer[0].timeStamp);
			ap->buffer[0].timeStamp = 0;
			ap->numSuccFrame++;
			ap->numTxFrame++;
			ap->numPrimFrame++;
			swapAp(ap);
		}else{
			printf("txAP's buffer is empty.\n");
			exit(5);
		}
		if(gSpec.fFd==false){
			pairSta = gSpec.numSTA;
		}else{
			if(gSpec.fOfdma==false){
				do{
					pairSta = rand() % gSpec.numSTA;
					j++;
					//Wrong
				}while(sta[pairSta].buffer[0].lengthMsdu==0 && j<100);
				if(j==100){
					pairSta = gSpec.numSTA;
				}
			}else{
				do{
					pairSta = rand() % gSpec.numSTA;
					j++;
					//Wrong
				}while(sta[pairSta].buffer[0].lengthMsdu==0 && j<100);
				if(j==100){
					pairSta = gSpec.numSTA;
				}
				j=0;
				do{
					pairSta2nd = rand() % gSpec.numSTA;
					j++;
				}while(((sta[pairSta2nd].buffer[0].lengthMsdu==0) || (pairSta==pairSta2nd))&&j<100);
				if(j==100){
					pairSta2nd = gSpec.numSTA;
				}
			}
		}
		for(i=0; i<gSpec.numSTA; i++){
			if((gSpec.fOfdma==true) && (i==pairSta2nd)){
				sta[i].retryCount = 0;
				sta[i].cw = gStd.cwMin;
				sta[i].backoffCount = rand() % (sta[i].cw + 1);
				sta[i].fSuccNow = true;
				sta[i].fTx = false;

				sta[i].sumFrameLengthInBuffer -= sta[i].buffer[0].lengthMsdu;
				sta[i].byteSuccFrame += sta[i].buffer[0].lengthMsdu;
				if(txFrameLength<sta[i].buffer[0].lengthMsdu){
					txFrameLength = sta[i].buffer[0].lengthMsdu;
				}
				sta[i].buffer[0].lengthMsdu = 0;
				//printf("%f\n", sta[i].buffer[0].timeStamp);
				sta[i].sumDelay += (gElapsedTime - sta[i].buffer[0].timeStamp);
				sta[i].buffer[0].timeStamp = 0;
				sta[i].numTxFrame++;
				sta[i].numSuccFrame++;
				swapSta(&sta[i]);
			}else if(i==pairSta){
				sta[i].retryCount = 0;
				sta[i].cw = gStd.cwMin;
				sta[i].backoffCount = rand() % (sta[i].cw + 1);
				sta[i].fSuccNow = true;
				sta[i].fTx = false;

				sta[i].sumFrameLengthInBuffer -= sta[i].buffer[0].lengthMsdu;
				sta[i].byteSuccFrame += sta[i].buffer[0].lengthMsdu;
				if(txFrameLength<sta[i].buffer[0].lengthMsdu){
					txFrameLength = sta[i].buffer[0].lengthMsdu;
				}
				sta[i].buffer[0].lengthMsdu = 0;
				sta[i].sumDelay += (gElapsedTime - sta[i].buffer[0].timeStamp);
				sta[i].buffer[0].timeStamp = 0;
				sta[i].numTxFrame++;
				sta[i].numSuccFrame++;
				swapSta(&sta[i]);
			}else{
				if(sta[i].buffer[0].lengthMsdu!=0){
					if(sta[i].backoffCount>0){
						sta[i].backoffCount--;
					}else{
						//printf("Really?\n");
						//Wrong
					}
				}
				sta[i].fSuccNow = false;
			}
		}

	}else{
		for(i=0; i<gSpec.numSTA; i++){
			if(sta[i].fTx==true){
				pairSta = i;
				//debugSta(&sta[pairSta]);
				break;
			}
		}
		if(gSpec.fOfdma==false){
			pairSta2nd = gSpec.numSTA;
		}else{
			do{
				pairSta2nd = rand() % gSpec.numSTA;
				j++;
			}while(((sta[pairSta2nd].buffer[0].lengthMsdu==0) || (pairSta==pairSta2nd))&&j<100);
			if(j==100){
				pairSta2nd = gSpec.numSTA;
			}
		}
		for(i=0; i<gSpec.numSTA; i++){
			if((i==pairSta) || (i==pairSta2nd)){
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
				if(txFrameLength<sta[i].buffer[0].lengthMsdu){
					txFrameLength = sta[i].buffer[0].lengthMsdu;
				}
				sta[i].buffer[0].lengthMsdu = 0;
				sta[i].sumDelay += (gElapsedTime - sta[i].buffer[0].timeStamp);
				sta[i].buffer[0].timeStamp = 0;
				sta[i].numTxFrame++;
				sta[i].numSuccFrame++;
				if(i==pairSta){
					sta[i].numPrimFrame++;
				}
				swapSta(&sta[i]);
			}else{
				if(sta[i].buffer[0].lengthMsdu!=0){
					if(sta[i].backoffCount>0){
						sta[i].backoffCount--;
					}else{
						//printf("Really?\n");
						//Wrong
					}
				}
				sta[i].fSuccNow = false;
			}
		}
		if(gSpec.fFd==true){
			if(ap->buffer[0].lengthMsdu!=0){
				ap->retryCount = 0;
				ap->cw = gStd.cwMin;
				ap->backoffCount = rand() % (ap->cw + 1);
				ap->fSuccNow = true;
				ap->fTx = false;

				ap->sumFrameLengthInBuffer -= ap->buffer[0].lengthMsdu;
				ap->byteSuccFrame += ap->buffer[0].lengthMsdu;
				if(txFrameLength<ap->buffer[0].lengthMsdu){
					txFrameLength = ap->buffer[0].lengthMsdu;
				}
				ap->buffer[0].lengthMsdu = 0;
				ap->sumDelay += (gElapsedTime - ap->buffer[0].timeStamp);
				ap->buffer[0].timeStamp = 0;
				ap->numSuccFrame++;
				ap->numTxFrame++;
				swapAp(ap);
			}else{
				ap->fSuccNow = false;
			}
		}else{
			if(ap->buffer[0].lengthMsdu!=0){
				if(ap->backoffCount>0){
					ap->backoffCount--;
				}else{
					//printf("Really?\n");
					//Wrong
				}
			}
			ap->fSuccNow = false;
		}
	}

	if(txFrameLength==0){
		printf("Frame length error\n");
		exit(12);
	}
	txTimeFrameLength = gStd.phyHeader + 4 * ((gStd.macService + 8* (gStd.macHeader + txFrameLength + gStd.macFcs) + gStd.macTail + (4 * gStd.dataRate - 1)) / (4 * gStd.dataRate));
	totalTime = txTimeFrameLength + gStd.sifs + gStd.timeAck;


	for(i=0; i<gSpec.numSTA; i++){
		arriveSta(&sta[i], (double)totalTime);
	}
	arriveAp(ap, (double)totalTime);

	*numTx = 0;
	gElapsedTime += (double)totalTime;
}

void afterSuccess(staInfo sta[], apInfo *ap, int *numTx){
	int minAfterSucc = 0;
	int i;
	*numTx = 0;

	for(i=0; i<gSpec.numSTA; i++){
		if(sta[i].buffer[0].lengthMsdu!=0){
			if(sta[i].fSuccNow==false){
				sta[i].afterSucc = sta[i].backoffCount * gStd.slot + gStd.afterSucc;
				minAfterSucc = sta[i].afterSucc;
			}else{
				sta[i].afterSucc = sta[i].backoffCount * gStd.slot + gStd.difs;
				minAfterSucc = sta[i].afterSucc;
			}
		}
	}

	if(ap->buffer[0].lengthMsdu!=0){
		if(ap->fSuccNow==false){
			//printf("ap, %d\n",ap->backoffCount);
			ap->afterSucc = ap->backoffCount * gStd.slot + gStd.afterSucc;
			minAfterSucc = ap->afterSucc;
		}else{
			ap->afterSucc = ap->backoffCount * gStd.slot + gStd.difs;
			minAfterSucc = ap->afterSucc;
		}
	}

	if(minAfterSucc==0){
		printf("All nodes' buffer is empty.\n");
		//Wrong
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
			if(sta[i].backoffCount<0){
				sta[i].backoffCount = 0;
				printf("error\n");
				//Wrong?
			}
         if(/*sta[i].backoffCount==0*/sta[i].afterSucc==minAfterSucc){
            (*numTx)++;
            sta[i].fTx = true;
         }else{
				sta[i].fTx = false;
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
		if(ap->backoffCount<0){
			ap->backoffCount = 0;
			printf("error\n");
		}
      if(/*ap->backoffCount==0*/ap->afterSucc==minAfterSucc){
         (*numTx)++;
         ap->fTx = true;
   	}else{
			ap->fTx = false;
		}
   }
   ap->fSuccNow = false;

	for(i=0; i<gSpec.numSTA; i++){
      arriveSta(&sta[i], (double)minAfterSucc);
   }
   arriveAp(ap, (double)minAfterSucc);

   gElapsedTime += (double)minAfterSucc;
}
