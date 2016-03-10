#!/bin/sh

numSta="2 10"
simTime=10
numTrial=10
trafficPattern=1
delayMode=0
fdMode=1
ofdmaMode=0
lambdaSta=0.0005
output="test"

for NUM_STA in $numSta
do
	if [ $fdMode -eq 1 ]
	then
		if [ $ofdmaMode -eq 1 ]
		then
			./FD_OFDMA --numSta=$NUM_STA --simTime=$simTime --trial=$numTrial --traffic=$trafficPattern --delay=$delayMode --fd --ofdma --lambdaSta=$lambdaSta --output=$output
		elif [ $ofdmaMode -eq 0 ]
		then
			./FD_OFDMA --numSta=$NUM_STA --simTime=$simTime --trial=$numTrial --traffic=$trafficPattern --delay=$delayMode --fd  --lambdaSta=$lambdaSta  --output=$output
		else
			echo "Error!"
			echo
		fi
	elif [ $fdMode -eq 0 ]
	then
		if [ $ofdmaMode -eq 1 ]
		then
			echo "Error!"
			echo
		elif [ $ofdmaMode -eq 0 ]
		then
			./FD_OFDMA --numSta=$NUM_STA --simTime=$simTime --trial=$numTrial --traffic=$trafficPattern --delay=$delayMode  --lambdaSta=$lambdaSta --output=$output
		else
			echo "Error!"
			echo
		fi
	else
		echo "Error!"
		echo
	fi
done

echo "Done."
