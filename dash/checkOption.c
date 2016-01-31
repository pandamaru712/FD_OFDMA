#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "checkOption.h"

void checkOption(int argc, char** argv){
	if(argc>1){
		char *help = argv[1];

		if(strcmp(help, "h")==0){
			printf(
			"2: 'h' shows help.\n"
			"3: IEEE 802.11 standard a/n/ac."
			"4: The number of STA.\n"
			"5: Simulation time (sec).\n"
			"6: test\n"
			);
		}
		exit(1);   //Showing help.
	}
}
