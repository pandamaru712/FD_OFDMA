#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setting.h"

int main(int argc, char *argv[]){
	if(argc>1){
		char *help = argv[1];

		if(strcmp(help, "h")==0){
			printf(
			"2: 'h' shows help.\n"
			"3: STA amount.\n"
			"4: Simulation time (sec).\n"
			);
		}
		exit(1);   //Showing help.
	}

	if(argc!=2){
		printf("The number of options is wrong\n");
		exit(2);
	}else{
		simSetting(argc, argv);
	}

	return 0;
}
