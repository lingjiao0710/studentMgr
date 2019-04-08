#ifndef _TEACHER_H_
#define _TEACHER_H_

#include "studentMgr.h"
#include "list.h"

int readTeacherList(List *tList);
int saveTeacherList(List *tList);
int listTeacher(List *tList);
int addTeacher(List *tList, char *name, char *phone);
int deleteTeacher(List *tList, char *id);
int getTeacher(List *tList, char *id, char *name, char *phone);
int editTeacher(List *tList, char *id, char *name, char *phone);
int doTeacher(char *mod, char *action);



#endif
