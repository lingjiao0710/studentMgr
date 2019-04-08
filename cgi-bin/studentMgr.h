#ifndef _STUDENTMGR_H_
#define _STUDENTMGR_H_
#include "cgic.h"

#define MAX_STR_LEN 1024
#define CMD_LEN 256
#define TEACHER_FILE "teacher.json"
#define CONTENTTYPEMSG "text/html; charset=utf-8"
#define TEACHER_HTML "../html/teacherInfo.html"
#define TEACHERADDNEW_HTML "../html/teacherAddNew.html"
#define TEACHEREDIT_HTML "../html/teacherEdit.html"

#define NETDISKLIST_HTML "../html/netdiskList.html"
#define NETDISKUPLOAD_HTML "../html/netdiskUpload.html"


#define printError(fmt...) do{cgiHeaderContentType(CONTENTTYPEMSG); fprintf(cgiOut, fmt);}while(0);


#endif
