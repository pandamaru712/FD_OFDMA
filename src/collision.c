#include <stdio.h>
#include <stdlib.h>
#include "setting.h"
#include "collision.h"
#include "bufferManager.h"

extern double gElapsedTime;
extern simSpec gSpec;
extern std11 gStd;

void txCollision(staInfo sta[], apInfo *ap){
	int i;
	int maxFrameLength = 0;
	int txTimeFrameLength = 0;

   for(i=0; i<gSpec.numSTA; i++){
      if(sta[i].buffer[0].lengthMsdu!=0){
         if(sta[i].fTx==true){
				if(maxFrameLength < sta[i].buffer[0].lengthMsdu){
					maxFrameLength = sta[i].buffer[0].lengthMsdu;
				}
            if(sta[i].cw<gStd.cwMax){
               sta[i].retryCount++;
               sta[i].cw = sta[i].cw * 2 + 1;
            }else if(sta[i].retryCount < gStd.retryLimit){
               sta[i].retryCount++;
            } else {
               sta[i].retryCount = 0;
               sta[i].cw = gStd.cwMin;
               sta[i].numLostFrame++;
					sta[i].sumFrameLengthInBuffer -= sta[i].buffer[0].lengthMsdu;
					sta[i].buffer[0].lengthMsdu = 0;
					sta[i].buffer[0].timeStamp = 0;
					swapSta(&sta[i]);
            }
            sta[i].backoffCount = rand() % (sta[i].cw + 1);
            sta[i].numTxFrame++;
            sta[i].numCollFrame++;
				sta[i].numPrimFrame++;
            sta[i].fCollNow = true;
            sta[i].fTx = false;
         }else{
            if(sta[i].backoffCount>0){
               sta[i].backoffCount--;
            }
         }
      }
   }

   if(ap->buffer[0].lengthMsdu!=0){
      if(ap->fTx==true){
			if(maxFrameLength<ap->buffer[0].lengthMsdu){
				maxFrameLength = ap->buffer[0].lengthMsdu;
			}
      	if(ap->cw < gStd.cwMax){
         	ap->retryCount++;
            ap->cw = ap->cw * 2 + 1;
         }else if(ap->retryCount < gStd.retryLimit){
            ap->retryCount++;
         }else{
            ap->retryCount = 0;
            ap->cw = gStd.cwMin;
            ap->numLostFrame++;
				ap->sumFrameLengthInBuffer -= ap->buffer[0].lengthMsdu;
				ap->buffer[0].lengthMsdu = 0;
				ap->buffer[0].timeStamp = 0;
				swapAp(ap);
         }
         ap->backoffCount = rand() % (ap->cw + 1);
         ap->numTxFrame++;
         ap->numCollFrame++;
			ap->numPrimFrame++;
         ap->fCollNow = true;
         ap->fTx = false;
      }else{
         if(ap->backoffCount>0){
            ap->backoffCount--;
         }
		}
   }

	txTimeFrameLength = gStd.phyHeader + 4 * ((gStd.macService + 8* (gStd.macHeader + maxFrameLength + gStd.macFcs) + gStd.macTail + (4 * gStd.dataRate - 1)) / (4 * gStd.dataRate));

   for(i=0; i<gSpec.numSTA; i++){
      arriveSta(&sta[i], txTimeFrameLength);
   }
   arriveAp(ap, txTimeFrameLength);

   gElapsedTime += (double)txTimeFrameLength;
}

void afterCollision(staInfo sta[], apInfo *ap, int *numTx){
	int i;
   int minAfterColl = 0;

   *numTx = 0;

   for(i=0; i<gSpec.numSTA; i++){
      if(sta[i].buffer[0].lengthMsdu!=0){
         if(sta[i].fCollNow==false){
            sta[i].afterColl = sta[i].backoffCount * gStd.slot + gStd.afterColl;
            minAfterColl = sta[i].afterColl;
         }else{
            sta[i].afterColl = sta[i].backoffCount * gStd.slot + gStd.ctsTimeout + gStd.difs;
            minAfterColl = sta[i].afterColl;
         }
      }
   }
   if(ap->buffer[0].lengthMsdu!=0){
      if(ap->fCollNow==false){
         ap->afterColl = ap->backoffCount * gStd.slot + gStd.afterColl;
         minAfterColl = ap->afterColl;
      }else{
         ap->afterColl = ap->backoffCount * gStd.slot + gStd.ctsTimeout + gStd.difs;
         minAfterColl = ap->afterColl;
      }
   }

   if(minAfterColl==0){
      printf("All nodes do not have a frame.\n");
   }

   for(i=0; i<gSpec.numSTA; i++){
      if((sta[i].afterColl < minAfterColl)&&(sta[i].buffer[0].lengthMsdu!=0)){
         minAfterColl = sta[i].afterColl;
      }
   }

   if((ap->afterColl<minAfterColl)&&(ap->buffer[0].lengthMsdu!=0)){
      minAfterColl = ap->afterColl;
   }

   for(i=0; i<gSpec.numSTA; i++){
      if(sta[i].buffer[0].lengthMsdu!=0){
         if(sta[i].fCollNow==false){
            if(minAfterColl>gStd.afterColl){
               sta[i].backoffCount -= ((minAfterColl - gStd.afterColl) / gStd.slot);
            }
         }else{
            if(minAfterColl > (gStd.ctsTimeout + gStd.difs)){
               sta[i].backoffCount -= ((minAfterColl - (gStd.ctsTimeout + gStd.difs)) / gStd.slot);
            }
         }
			if(sta[i].backoffCount<0){
				sta[i].backoffCount = 0;
				//Wrong?
			}
      	if(sta[i].backoffCount==0/*sta[i].afterColl==minAfterColl*/){
            (*numTx)++;
            sta[i].fTx = true;
         }else{
				sta[i].fTx = false;
			}
         sta[i].fCollNow = false;
      }
   }

   if(ap->buffer[0].lengthMsdu!=0){
      if(ap->fCollNow==false){
         if(minAfterColl>gStd.afterColl){
            ap->backoffCount -= ((minAfterColl - gStd.afterColl) / gStd.slot);
         }
      }else{
         if(minAfterColl > (gStd.ctsTimeout + gStd.difs)){
            ap->backoffCount -= ((minAfterColl - (gStd.ctsTimeout + gStd.difs)) / gStd.slot);
         }
      }
		if(ap->backoffCount<0){
			ap->backoffCount = 0;
		}
      if(ap->backoffCount==0/*ap->afterColl==minAfterColl*/){
         (*numTx)++;
         ap->fTx = true;
      }else{
			ap->fTx = false;
		}
      ap->fCollNow = false;
   }

   for(i=0; i<gSpec.numSTA; i++){
      arriveSta(&sta[i], minAfterColl);
   }
   arriveAp(ap, minAfterColl);

   gElapsedTime += (double)minAfterColl;

}
