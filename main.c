#include <stdio.h>
#include <string.h>
#include "ConsoleMode.h"
#include "GUI_Manager.h"

int main(int argc, char *argv[]){

	if (argc==0){
		printf("%s",argv[0]);
	}

	if (argc == 2 && strcmp(argv[1],"-g")==0){
		guiMain();
	}
	else {
		setbuf(stdout, NULL);
		consoleMain();
	}
	//status is SP_GAME_STATUS_QUIT
	return 1;
}

