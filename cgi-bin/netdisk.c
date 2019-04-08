#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/file.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <time.h>

#include "cgic.h"
#include "ctemplate.h"
#include "studentMgr.h"

#define NETDISK_DIR "../html/netdisk"
#define KB 1024
#define MB 1048576
#define GB 1073741824

int makeFileSize(char *fileSize, off_t size)
{
	if(size <= KB){
		sprintf(fileSize, "%d 字节", size);
	}else if(size <= MB){
		sprintf(fileSize, "%d KB", size / KB);
	}else if(size < GB){
		sprintf(fileSize, "%d MB", size / MB);
	}else{
		sprintf(fileSize, "%d GB", size / GB);
	}

	return 0;
}

int netdiskList(const char *path)
{
	char filename[CMD_LEN] = {0};
	char timebuf[CMD_LEN] = {0};
	char fileSize[CMD_LEN] = {0};
	int ret;
	//long clear_size = -1;
	DIR* pDir = NULL;
	struct dirent* ent = NULL;
	struct stat buf;
	struct tm *p = NULL;
	time_t timep = 0;

	TMPL_varlist *mainList=0;
	TMPL_loop *loop = 0;

	//fprintf(cgiOut, "\r\n\r\n");
	if(lstat(path, &buf) == -1)
	{
		printf("get %s stat failed\n", path);
		return -1;
	}
	
	if(S_ISDIR(buf.st_mode))
	{
		pDir = opendir(path);
		if(!pDir)
		{
			printf("opendir %s failed\n", path);
			return -1;
		}
		while(NULL != (ent=readdir(pDir)))
		{
			if(ent->d_name[0] == '.'){
				continue;
			}
			bzero(filename, sizeof(filename));
			sprintf(filename, "%s/%s", path, ent->d_name);
			if(lstat(filename, &buf) == -1)//
			{
				printf("get %s stat failed\n", filename);
				continue;
			}
			if(S_ISDIR(buf.st_mode))
			{
				continue;
			}

			timep = buf.st_ctime;
			p = localtime((const time_t *)&timep);
			sprintf(timebuf, "%d-%d-%d %02d:%02d:%02d", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
			//fprintf(cgiOut, "get %s filesize:%d, ctime:%s\n", ent->d_name, buf.st_size, timebuf);

			
			makeFileSize(fileSize, buf.st_size);
			loop = TMPL_add_varlist(loop, TMPL_add_var(NULL, "fileName", ent->d_name, "createTime", timebuf, "fileSize", fileSize, NULL));
			//clear_size += buf.st_size;
		}
		closedir(pDir);
	}

	mainList = TMPL_add_loop(mainList,"files",loop);
	cgiHeaderContentType(CONTENTTYPEMSG);
	fprintf(cgiOut, "\r\n\r\n");
	TMPL_write(NETDISKLIST_HTML, 0, 0, mainList,cgiOut,cgiOut);
	TMPL_free_varlist(mainList);

	return 0;
}

int netdiskUploadSubmit(void)
{
	char fileName[CMD_LEN] = {0};
	char destfileName[CMD_LEN] = {0};
	int fileSize = 0;
	int got;
	char buffer[2048] = {0};
	cgiFilePtr file = NULL;
	FILE *fp = NULL;

	if(cgiFormFileName("file", fileName, sizeof(fileName)) != cgiFormSuccess){
		printError("文件名获取错误 ！");
		goto err;
	}

	cgiFormFileSize("file", &fileSize);
	if(fileSize > 2 * MB){
		printError("文件大小不能超过2MB");
		goto err;
	}

	if(cgiFormFileOpen("file", &file) != cgiFormSuccess){
		
		printError("打开文件 %s 失败！", fileName);
		goto err;
	}

	sprintf(destfileName, "%s/%s", NETDISK_DIR, fileName);

	fp = fopen(destfileName, "wb");
	while(cgiFormFileRead(file, buffer, sizeof(buffer), &got) == cgiFormSuccess){
		if(got > 0){
			fwrite(buffer, 1, got, fp);
		}
	}

	if(file){
		cgiFormFileClose(file);
		file = NULL;
	}
	if(fp){
		fclose(fp);
		fp = NULL;
	}

	cgiHeaderLocation("studentMgr.cgi?mod=netdisk&action=list");

	return 0;
err:
	if(file){
		cgiFormFileClose(file);
		file = NULL;
	}
	if(fp){
		fclose(fp);
		fp = NULL;
	}

	return -1;
}

int doNetDisk(char *mod, char *action)
{	
	//sleep(1);

	if(0 == strcmp(action, "list")){
		netdiskList(NETDISK_DIR);
	}else if(strcmp(action,"upload")==0)
	{
		cgiHeaderContentType(CONTENTTYPEMSG);
		fprintf(cgiOut, "\r\n\r\n");
		TMPL_write(NETDISKUPLOAD_HTML,0,0,0,cgiOut,cgiOut);
	}else if(strcmp(action,"delete")==0){
		char fileName[CMD_LEN] = {0};
		char filePath[CMD_LEN]= {0};
		cgiFormString("fileName",fileName, sizeof(fileName));
		sprintf(filePath,"%s/%s",NETDISK_DIR, fileName);
		unlink(filePath);
		cgiHeaderLocation("studentMgr.cgi?mod=netdisk&action=list");
	}else if(strcmp(action,"uploadSubmit")==0){
		netdiskUploadSubmit();
	}
}
/*
int cgiMain()
{

	netdiskList(NETDISK_DIR);

	return 0;
}
*/
