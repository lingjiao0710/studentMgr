#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgic.h"
#include "ctemplate.h"
#include "cJSON.h"
#include "teacher.h"


//#define DEBUG


int cgiMain()
{
	char mod[CMD_LEN] = {0};
	char action[CMD_LEN] = {0};

#ifndef DEBUG 
	cgiFormString("mod",mod,sizeof(mod));
	cgiFormString("action",action,sizeof(action));
#else
	sprintf(mod, "%s", "netdisk");
	sprintf(action, "%s", "list");	

#endif

	if(0 == strcmp(mod, "teacher")){
		doTeacher(mod, action);
	}else if(0 == strcmp(mod, "student")){
		//doStudent(mod, action);
		;
	}else if(0 == strcmp(mod, "netdisk")){
		doNetDisk(mod, action);
	}
	


	return 0;
}
