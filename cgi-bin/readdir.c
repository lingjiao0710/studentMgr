#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/file.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define RETURN_OK 0
#define RETURN_FAILED -1
#define FILE_NAME_MAX 256


int ReadDirSize(const char *path)
{
	char filename[FILE_NAME_MAX] = {0};
	int ret;
	long clear_size = -1;
	DIR* pDir = NULL;
	struct dirent* ent = NULL;
	struct stat buf;
	int i;

	if(lstat(path, &buf) == -1)
	{
		printf("get %s stat failed\n", path);
		return RETURN_FAILED;
	}
	
	if(S_ISDIR(buf.st_mode))
	{
		pDir = opendir(path);
		if(!pDir)
		{
			printf("opendir %s failed\n", path);
			return RETURN_FAILED;
		}
		while(NULL != (ent=readdir(pDir)))
		{
			if(ent->d_name[0] == '.'){
				continue;
			}
			bzero(filename, sizeof(filename));
			sprintf(filename, "%s%s", path, ent->d_name);
			if(lstat(filename, &buf) == -1)//lstat要求绝对路径
			{
				printf("get %s stat failed\n", filename);
				continue;
			}
			if(S_ISDIR(buf.st_mode))
			{
				continue;
			}

			printf("get %s filesize:%d\n", filename, buf.st_size);
			clear_size += buf.st_size;
		}
		closedir(pDir);
	}

	return clear_size;
}


int main(int argc, char *argv[])
{
	int totalsize = 0;

	totalsize = ReadDirSize(argv[1]);

	printf("totalsize:%d\n", totalsize);

	return 0;
}
