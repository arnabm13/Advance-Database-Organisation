/*
 * storage_managerfunction.c
 *
 *  Created on: Sep 5, 2015
 *      Author: ANB
 */
#ifndef storage_managerfunction
#define storage_managerfunction
#include<malloc.h>
//#include "dberror.h"
#include "storage_mgr.h"
#include <string.h>
//SM_FileHandle *fileHandle;

void
initStorageManager(void){

}

RC createPageFile(char *fileName){
    FILE *file;
	file = fopen(fileName, "w+b"); //create a file
	fseek(file,PAGE_SIZE, 0);
	fputc('\0', file);
	if (file==NULL)
		return RC_FILE_NOT_FOUND;
	fclose(file);
	return RC_OK;
}

RC openPageFile (char *fileName,SM_FileHandle *fileHandle) {

SM_FileHandle temp;
FILE *fo;
temp=*fileHandle; //creating a local copy of fileHandle
long filend = 0;

    fo = fopen(fileName,"r+");
	if(fo==NULL) // checks if the pointer contains the address to a file
	{
		return RC_FILE_NOT_FOUND;
	}

	temp.fileName=fileName;
	temp.curPagePos=0;
	fseek( fo, 0L, SEEK_END );
	filend = ftell(fo);
	temp.totalNumPages = filend/PAGE_SIZE;
     *fileHandle = temp; //Assigning back the local copy to the fileHandle
	fclose(fo);
return RC_OK;
}

RC destroyPageFile (char * filename)
{
	FILE *fd;
printf("file to be destroyed is %s",filename);
	fd = fopen(filename,"r+");
    
	fclose(fd);
    remove(filename);
    return RC_OK;

}

RC closePageFile(SM_FileHandle *fhandler){
	SM_FileHandle temp;
	printf("Im in close file");
        temp = *fhandler;
	temp.curPagePos=-1;
	temp.totalNumPages=-1;
	*fhandler = temp;
	return RC_OK;
}

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
FILE *fp= NULL;
SM_FileHandle temp;
temp=*fHandle; //creating a local copy of fileHandle
char ch[4096];

fp = fopen(fHandle->fileName,"r+b");
if(fp==NULL)
	printf("File not found");

if(fHandle->totalNumPages<pageNum)
			{
				return RC_READ_NON_EXISTING_PAGE;
			}

	    rewind(fp);
		fseek(fp,PAGE_SIZE,PAGE_SIZE*(pageNum-1));
		fread(ch,PAGE_SIZE,1,fp);
		memPage = ch;
		puts(memPage);
		temp.curPagePos= (pageNum-1);
        *fHandle=temp; //Assigning back the local copy to the fileHandle
		fclose(fp);
		return RC_OK;
}
int getBlockPos (SM_FileHandle *fHandle){
	return fHandle->curPagePos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	FILE *fp= NULL;
	SM_FileHandle temp;
	temp=*fHandle; //creating a local copy of fileHandle
	char ch[4096];

	fp = fopen(fHandle->fileName,"r+b");
	rewind(fp);
	int r= fread(ch,PAGE_SIZE,1,fp);
    memPage= ch;

	if(r==0)
		{
			if(feof(fp)){fclose(fp);}
			printf("file cannot read\n");
		}

	//fHandle->curPagePos=0;
	temp.curPagePos=0;
    *fHandle=temp;
	return RC_OK;
}
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	FILE *fp= NULL;
	SM_FileHandle temp;
    temp=*fHandle;
    int curPos=fHandle->curPagePos;

    fp = fopen(fHandle->fileName,"r+b");
	if((curPos-1)<0)
		return RC_READ_NON_EXISTING_PAGE;
    rewind(fp);
	fseek(fp,((curPos-1)*PAGE_SIZE),0);
	int r= fread(memPage,PAGE_SIZE,1,fp);

	if(r==0)
			{
				if(feof(fp)){fclose(fp);}
				printf("file cannot read\n");
			}
	temp.curPagePos=curPos-1;
	*fHandle=temp;
	return RC_OK;
}
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	FILE *fp;
    char ch[4096];

	fp = fopen(fHandle->fileName,"r+b");
	int curPos=fHandle->curPagePos;
	rewind(fp);
	fseek(fp,(curPos*PAGE_SIZE),0);
	int r= fread(ch,PAGE_SIZE,1,fp);
    memPage=ch;

	if(r==0)
	{
		if(feof(fp)){fclose(fp);}
		printf("file cannot read\n");
	}
	return RC_OK;
}
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	FILE *fp= NULL;
	char ch[4096];
	SM_FileHandle temp;
	temp=*fHandle;
	int curPos = temp.curPagePos;

	fp = fopen(temp.fileName,"r+b");
	rewind(fp);
	fseek(fp,((curPos+1)*PAGE_SIZE),0);
	int r=fread(memPage,PAGE_SIZE,1,fp);
	memPage= ch;

	if(r==0)
	{
		if(feof(fp)){fclose(fp);}
		printf("file cannot read\n");
	}
	temp.curPagePos=curPos+1;
	*fHandle=temp;
	return RC_OK;
}
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	FILE *fp= NULL;
	char ch[4096];
	SM_FileHandle temp;
	temp=*fHandle;

	fp = fopen(fHandle->fileName,"r+b");
	fseek(fp,(fHandle->totalNumPages-1)*PAGE_SIZE, 0);
	int r=fread(memPage,PAGE_SIZE,1,fp);
	memPage= ch;

	if(r==0)
	{
		if(feof(fp)){fclose(fp);}
		printf("file cannot read\n");
	}
	temp.curPagePos=temp.totalNumPages;
	*fHandle=temp;
	return RC_OK;

}
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
FILE* fw;
SM_FileHandle temp;
temp=*fHandle;
fw= fopen(temp.fileName,"w+b");

	fseek(fw, pageNum*PAGE_SIZE, 0);           //move pointer to point the pageNumth page
	int w=fwrite(memPage,PAGE_SIZE,1,fw);       //write content of memPage into block
	if(w==0)                                    //check if block can be written
	{
		printf("file write error\n");
	}

	temp.totalNumPages = pageNum+1;
	*fHandle=temp;
	return RC_OK;
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	FILE* fc;
	SM_FileHandle temp;
	temp=*fHandle;

	fc= fopen(temp.fileName,"a+");
	int curPagePos = temp.curPagePos+1;
    rewind(fc);
	fseek(fc,(curPagePos*PAGE_SIZE),1);                    //move pointer to point the current page
	int w= fwrite(memPage,1,PAGE_SIZE,fc);                                 //write content of memPage into block

	if(w==0)                                    //check if block can be written
	{
		printf("file write error\n");
	}
	rewind(fc);
    temp.curPagePos=curPagePos;
    temp.totalNumPages=temp.totalNumPages + 1;

    *fHandle=temp;

	return RC_OK;
}
RC appendEmptyBlock (SM_FileHandle *fHandle){
	FILE* fc;
	SM_FileHandle temp;
    temp=*fHandle;
		fc= fopen(fHandle->fileName,"a+");
		rewind(fc);
	fseek(fc,(fHandle->totalNumPages*PAGE_SIZE),0);            //move pointer to point the beginning of the appending block
	char newblock[PAGE_SIZE] = {0};                           //the new last page should be filled with zero bytes
	int w = fwrite(newblock, PAGE_SIZE,1,fc);                            //write new last page
	if(w==0)                                    //check if block can be written
		{
			printf("file write error\n");
		}
	//update fHandle
	temp.curPagePos=temp.totalNumPages+1;                //move curPagePos to the last new block
	temp.totalNumPages=temp.totalNumPages+1;             //increase total number pages
	*fHandle=temp;
	int size = ftell(fc)/PAGE_SIZE;

	return size;
}
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
	SM_FileHandle temp;
    temp=*fHandle;
	if(fHandle->totalNumPages<numberOfPages)
	{
		//fseek(fp,(fHandle->totalNumPages*PAGE_SIZE),0);
		temp.totalNumPages=numberOfPages;                          //increase total number pages to numberOfPages
	}
	*fHandle=temp;
	return RC_OK;
}

#endif
