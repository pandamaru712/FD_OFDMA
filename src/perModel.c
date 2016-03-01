#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nodeInfo.h"
#include "perModel.h"

double calculateRSSI(apInfo *ap, staInfo *sta, bool isTxSta){
	double RSSI;
	double distance;

	distance = sqrt(pow(ap->x - sta->x, 2) + pow(ap->y - sta->y, 2));
	if(isTxSta==false){
		RSSI = ap->txPower + ap->antennaGain + sta->antennaGain - (30*log10(distance) + 47);
	}else{
		RSSI = sta->txPower + ap->antennaGain + sta->antennaGain - (30*log10(distance) + 47);
	}
	return RSSI;
}
