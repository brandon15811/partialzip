#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "partial/partial.h"
#define SIZE 40

char endianness = IS_LITTLE_ENDIAN;

size_t data_callback(ZipInfo* info, CDFile* file, unsigned char *buffer, size_t size, void *userInfo)
{
	char **pos = userInfo;
	memcpy(*pos, buffer, size);
	*pos += size;
}

int main(int argc, char* argv[]) {
	char * zipurl;
	FILE *file;
	char saveFile[1000];
	char savePath[1000];
	FILE *fp;
	char buf[1000];
	int len;
	
	if (argc != 2)
	{
		printf("usage: %s <textfile with source and destination on seperate lines>\n", argv[0]);
		return 1;
	}
	
	zipurl = getenv ("zipurl");
	if (!zipurl)
	{
		printf ("Set $zipurl to a zip url (or a file as a file:// url)\n");
		return 1;
	}
	
	ZipInfo* info = PartialZipInit(zipurl);
	if(!info)
	{
		printf("1Cannot find %s\n", zipurl);
		return 1;
	}
	
	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		printf("1Cannot find %s\n", argv[1]);
		return (0);
	}
	
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		char* zipPathTrimmed = buf + 1;
		len = strlen(zipPathTrimmed);
		if(zipPathTrimmed[len-1] == '\n')
		zipPathTrimmed[len-1] = 0;
		CDFile* filezip = PartialZipFindFile(info, zipPathTrimmed);
		if(!filezip)
		{
			printf("2Cannot find %s in %s|\n", zipPathTrimmed, zipurl);
			fgets(buf, sizeof(buf), fp);
			continue;
		}
		
		int dataLen = filezip->size;
		unsigned char *data = malloc(dataLen+1);
		unsigned char *pos = data;
		PartialZipGetFile(info, filezip, data_callback, &pos);
		*pos = '\0';
		
		sprintf(saveFile, "%s", strrchr(buf, '/'));
		fgets(buf, sizeof(buf), fp);
		len = strlen(buf);
		if(buf[len-1] == '\n')
		buf[len-1] = 0;
		
		char* saveFileTrimmed = saveFile + 1;
		
		sprintf(savePath, "%s/%s", buf, saveFileTrimmed);
		
		printf("SavePath: %s|\n", savePath);
		file = fopen(savePath,"w");
		fwrite(data, 1, dataLen, file);
		fclose(file);
		free(data);
		
	}
	PartialZipRelease(info);
	return 0;
}

