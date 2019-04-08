#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "studentMgr.h"

int InitList(List *tList)
{
    tList->next = tList;
    tList->prev = tList;
    tList->data = NULL;

    return 0;
}

int EnListHead (List *tList, void *data)
{
    List *tl;
	
    tl = malloc(sizeof(List));
    if (!tl){
		printf("EnListHead fail  \r\n");
		return  -1;
    }
    tl->next = tList->next;
    tl->prev = tList;
    tList->next->prev = tl;
    tList->next = tl;
	
    tl->data = data;
    return 0;
}

int EnListTail(List *tList, void *data)
{
    List *tl;
	
    tl = malloc(sizeof(List));
    if (!tl) 
    {
        printf("EnListTail fail  \r\n");
        return -1;
    }
    tl->next = tList;
    tl->prev = tList->prev;
    tList->prev->next = tl;
    tList->prev = tl;
	
    tl->data = data;
    return 0;
}

int RemoveList(List *tList, void *data)
{
    List *tl;
	
    for (tl = tList->next; tl != tList; tl=tl->next) {
        if (tl->data == data) {
            tl->next->prev = tl->prev;
            tl->prev->next = tl->next;
            if(tl){
            	free(tl);
				tl = NULL;
			}
            return 0;
        }
    }
    return -1;
}


/* 文件操作，读取文件weaid.json */
char * readJsonFile(char *file)
{
	FILE *fp = NULL;
	int filesize = 0;
	int weatherID = -1;
	char *file_buf = NULL;
	char buffer[MAX_STR_LEN] = {0};
	
	fp = fopen(file, "r");
	if(fp){
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if(filesize > 0){
			file_buf = calloc(filesize + 1, 1);
			if(file_buf){
				while(fgets(buffer, sizeof(buffer), fp)){
					strcat(file_buf, buffer);
				}
			}
		}	
	}
	
	if(fp){
		fclose(fp);
		fp = NULL;
	}

	return file_buf;
}

int saveJsonFile(char *jsonData, char *file)
{
	FILE *fp = NULL;
	
	fp = fopen(file, "w");
	if(fp){
		fputs(jsonData, fp);
	}
	
	if(fp){
		fclose(fp);
		fp = NULL;
	}

	return 0;
}


