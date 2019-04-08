#ifndef _LIST_H_
#define _LIST_H_
typedef struct list{
	struct list *next;     
	struct list *prev;     
	void *data;
}List;

int InitList(List *tList);

int EnListHead (List *tList, void *data);
int EnListTail(List *tList, void *data);
int RemoveList(List *tList, void *data);

char * readJsonFile(char *file);
int saveJsonFile(char *jsonData, char *file);

#endif

