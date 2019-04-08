#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgic.h"
#include "ctemplate.h"
#include "cJSON.h"
#include "teacher.h"

static List gTeacherList;
static int gTeacherListId = 1;


typedef struct teacher{
	char id[CMD_LEN];
	char name[MAX_STR_LEN];
	char phone[MAX_STR_LEN];

}Teacher;



int readTeacherList(List *tList)
{
	char *jsonData = NULL;
	
	jsonData = readJsonFile(TEACHER_FILE);
	
	if(jsonData == NULL){
		return -1;
	}

	int i = 0;
	cJSON *json = cJSON_Parse(jsonData);
	if(json){
		cJSON *data = cJSON_GetObjectItem(json, "data");
		if(data){
			int count = cJSON_GetArraySize(data);
			//fprintf(cgiOut, "count:%d\n", count);
			for(i = 0; i < count; i++){
				cJSON *item = cJSON_GetArrayItem(data, i);
				if(item){
					Teacher *node = calloc(1, sizeof(Teacher));
					cJSON *id = cJSON_GetObjectItem(item, "id");
					if(id){
						strcpy(node->id, id->valuestring);;						
						gTeacherListId = atoi(node->id) + 1; 
						//fprintf(cgiOut, "gTeacherListId :%d\n", gTeacherListId);
					}
					cJSON *name = cJSON_GetObjectItem(item, "name");
					if(name){
						//fprintf(cgiOut, "name :%s\n", name->valuestring);
						strcpy(node->name, name->valuestring);
					}
					cJSON *phone = cJSON_GetObjectItem(item, "phone");
					if(phone){
						//fprintf(cgiOut, "phone :%s\n", phone->valuestring);
						strcpy(node->phone, phone->valuestring);
					}
					EnListTail(tList, node);
				}
			}
		}
	}

	if (json){	
		cJSON_Delete(json);  
		json = NULL;  
	} 

	return 0;
}

int saveTeacherList(List *tList)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	cJSON *field = NULL;
	List *list = tList->next;
	
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "data", data=cJSON_CreateArray());

	for(list; list != tList; list=list->next){
		if(list->data){
			Teacher *node = list->data;
			cJSON_AddItemToArray(data, field=cJSON_CreateObject());
			cJSON_AddStringToObject(field, "id",  node->id);
			cJSON_AddStringToObject(field, "name",  node->name);
			cJSON_AddStringToObject(field, "phone",  node->phone);
		}
	}

	char *cjson_out = NULL;
    cjson_out=cJSON_Print(root);
	if(cjson_out){
		saveJsonFile(cjson_out, TEACHER_FILE);
		free(cjson_out);
		cjson_out = NULL;
	}

	return 0;
}

int listTeacher(List *tList)
{
	TMPL_varlist *v1 = NULL;
	TMPL_varlist *mainlist = NULL;
	TMPL_loop *loop = NULL;
	List *list = tList->next;
	char tmp[CMD_LEN] = {0};
	Teacher *node = NULL;

	cgiHeaderContentType(CONTENTTYPEMSG);
	fprintf(cgiOut, "\r\n\r\n");
//	fprintf(cgiOut, "test1111\n");

	for(; list != tList; list=list->next){
		if(list->data){
			node = list->data;
			//printError(tmp);
			loop = TMPL_add_varlist(loop, TMPL_add_var(NULL, "name", node->name, "phone", node->phone, "id", node->id, NULL));
			//fprintf(cgiOut, "id:%s, name:%s, phone:%s\n", node->id, node->name, node->phone);
			//v1 = TMPL_add_var(0, "id", tmp, 0);
			//loop = TMPL_add_varlist(loop, v1);
			//v1 = TMPL_add_var(0, "name", node->name, "phone", node->phone, 0);
			//loop = TMPL_add_varlist(loop, v1);
		}
	}

	mainlist = TMPL_add_loop(0, "teachers", loop);


	TMPL_write(TEACHER_HTML,0,0,mainlist,cgiOut,cgiOut);

	TMPL_free_varlist(mainlist);
}

int addTeacher(List *tList, char *name, char *phone)
{
	Teacher *node = calloc(1, sizeof(Teacher));

	sprintf(node->id, "%d", gTeacherListId);
	strncpy(node->name, name, sizeof(node->name) - 1);
	strncpy(node->phone, phone, sizeof(node->phone) - 1);

	EnListTail(tList, node);

	saveTeacherList(tList);

	return 0;
}

int deleteTeacher(List *tList, char *id)
{
	Teacher *node = NULL;
	List *list = tList->next;
	
	for(; list != tList; list=list->next){
		if(list->data){
			node = list->data;
			if(node->id && id && 0 == strcmp(node->id, id)){
				RemoveList(tList, node);
				return 0;
			}		
		}
	}

	return 0;
}

int getTeacher(List *tList, char *id, char *name, char *phone)
{
	Teacher *node = NULL;
	List *list = tList->next;
	
	for(; list != tList; list=list->next){
		if(list->data){
			node = list->data;
			if(node->id && id && 0 == strcmp(node->id, id)){
				strcpy(name, node->name);
				strcpy(phone, node->phone);
				return 0;
			}		
		}
	}

	return 0;
}

int editTeacher(List *tList, char *id, char *name, char *phone)
{
	Teacher *node = NULL;
	List *list = tList->next;
	
	for(; list != tList; list=list->next){
		if(list->data){
			node = list->data;
			if(node->id && id && 0 == strcmp(node->id, id)){
				strcpy(node->name, name);
				strcpy(node->phone, phone);
				return 0;
			}		
		}
	}

	return 0;
}


int doTeacher(char *mod, char *action)
{
	char id[CMD_LEN] = {0};
	char name[MAX_STR_LEN] = {0};
	char phone[MAX_STR_LEN] = {0};

	//初始化链表
	InitList(&gTeacherList);
	//读取数据到链表中
	readTeacherList(&gTeacherList);

	//usleep(1);

	if(0 == strcmp(action, "list")){
		listTeacher(&gTeacherList);
	}else if(0 == strcmp(action, "addnew")){
		cgiHeaderContentType(CONTENTTYPEMSG);
		TMPL_write("../html/teacherAddNew.html",0,0,0,cgiOut,cgiOut);
	}else if(0 == strcmp(action, "adddata")){

		cgiFormString("name", name, sizeof(name));
		cgiFormString("phone", phone, sizeof(phone));

		if(name[0] == 0 || phone[0] == 0){
			cgiHeaderLocation("studentMgr.cgi?mod=teacher&action=list");
			return 0;
		}
		//fprintf(cgiOut,"%s\n",name);
		//fprintf(cgiOut,"%s\n",phone);
		usleep(1);
		addTeacher(&gTeacherList, name, phone);
		saveTeacherList(&gTeacherList);
		cgiHeaderLocation("studentMgr.cgi?mod=teacher&action=list");
	}else if(0 == strcmp(action, "delete")){
		#if 1
		cgiFormString("id", id, sizeof(id));
		#else
		sprintf(id, "%s", "1");
		#endif
		//printError(id);
		usleep(1);
		deleteTeacher(&gTeacherList, id);
		saveTeacherList(&gTeacherList);
		cgiHeaderLocation("studentMgr.cgi?mod=teacher&action=list");
	}else if(0 == strcmp(action, "edit")){
#ifndef DEBUG 
		cgiFormString("id", id, sizeof(id));
#else
		sprintf(id, "%s", "1");
#endif
		//printError(id);
		usleep(1);
		getTeacher(&gTeacherList, id, name, phone);

		//fprintf(cgiOut, "id:%s, name :%s phone:%s\n", id, name, phone);
		if(id[0] == 0 || name[0] == 0 || phone[0] == 0){
			cgiHeaderLocation("studentMgr.cgi?mod=teacher&action=list");
			return 0;
		}

		cgiHeaderContentType(CONTENTTYPEMSG);
		fprintf(cgiOut, "\r\n\r\n");
		TMPL_varlist *mainlist = NULL;
		mainlist = TMPL_add_var(mainlist,"id", id, "name", name, "phone", phone, NULL);
		TMPL_write(TEACHEREDIT_HTML,0,0,mainlist,cgiOut,cgiOut);
		TMPL_free_varlist(mainlist);
	}else if(0 == strcmp(action, "editdata")){
		cgiFormString("id", id, sizeof(id));
		cgiFormString("name", name, sizeof(name));
		cgiFormString("phone", phone, sizeof(phone));

		
#ifdef DEBUG
		sprintf(id, "%s", "1");
		fprintf(cgiOut, "id:%s, name :%s phone: %s\n", id, name, phone);

#endif
		if(id[0] == 0 || name[0] == 0 || phone[0] == 0){
			cgiHeaderLocation("studentMgr.cgi?mod=teacher&action=list");
			return 0;
		}

		usleep(1);

		editTeacher(&gTeacherList, id, name, phone);
		saveTeacherList(&gTeacherList);
		cgiHeaderLocation("studentMgr.cgi?mod=teacher&action=list");
	}

}
