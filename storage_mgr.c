#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<malloc.h>
//#include "dberror.h"
#include "storage_mgr.h"
#include <string.h>
//SM_FileHandle *fileHandle;
RC appendEmptyBlock (SM_FileHandle *fHandle);
void
initStorageManager(void){

}

RC createPageFile(char *fileName){
        FILE *file;
	file = fopen(fileName, "w"); 
	char *t_page, *f_page;
        t_page = (char *)malloc(PAGE_SIZE * sizeof(char));  // Allocating memory to the first page. The page contains the numper of page info
        f_page = (char *)malloc(PAGE_SIZE * sizeof(char)); // Allocating the second page as the first page.
        strcat(t_page,"1\n");
        fwrite(t_page, sizeof(char), PAGE_SIZE, file);
        fwrite(f_page, sizeof(char), PAGE_SIZE, file);
        
	if (file==NULL)
        return RC_FILE_NOT_FOUND;
        free(t_page);
        free(f_page);
	fclose(file);
        return RC_OK;
}

RC openPageFile (char *fileName,SM_FileHandle *fileHandle) {

FILE *pagef = fopen(fileName, "r+");

    if (pagef){

        char *str;

        str = (char *) calloc(PAGE_SIZE, sizeof(char));
        fgets(str, PAGE_SIZE, pagef);

        str = strtok(str, "\n"); /* remove trailing newline char */

        fileHandle->fileName = fileName; /* assign values to SM_FileHandle struct components */
        fileHandle->totalNumPages = atoi(str);
        fileHandle->curPagePos = 0;
        fileHandle->mgmtInfo = pagef;
         free(str);
        return RC_OK;
    }
    return RC_FILE_NOT_FOUND;
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
	//printf("Im in close file");
        temp = *fhandler;
	temp.curPagePos=-1;
	temp.totalNumPages=-1;
	*fhandler = temp;
	return RC_OK;
}

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
FILE *fp= NULL;
SM_FileHandle temp;
long filend;
temp=*fHandle; //creating a local copy of fileHandle


if (fHandle->mgmtInfo == NULL){
        printf("File not found");
        return RC_FILE_NOT_FOUND;
    }
	

if(pageNum > fHandle->totalNumPages|| pageNum < 0)
			{
				return RC_READ_NON_EXISTING_PAGE;
			}
int returnvalue = fseek(fHandle->mgmtInfo, (pageNum+1)*PAGE_SIZE*sizeof(char), SEEK_SET);
          if (returnvalue==0){
                fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
                fHandle->curPagePos = pageNum;	
		return RC_OK;
             
          }

   return RC_READ_NON_EXISTING_PAGE;
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

int returnvalue;
if (pageNum>(fHandle->totalNumPages)||pageNum<0)
return RC_WRITE_FAILED;

	fseek(fHandle->mgmtInfo, (pageNum+1)*PAGE_SIZE*sizeof(char), SEEK_SET);           //move pointer to point the pageNumth page
	fwrite(memPage,sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);       //write content of memPage into block
	fHandle->curPagePos = pageNum;
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

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
	SM_FileHandle temp;
    
	if (fHandle->totalNumPages < numberOfPages){
		int numPages = numberOfPages - fHandle->totalNumPages; /* calculates number of pages required to meet the required size of the file */
        int i;
        for (i=0; i < numPages; i++){
			appendEmptyBlock(fHandle); /* increases the size of the file to required size by appending required pages. */
		}
    }
    return RC_OK;
	
	return RC_OK;
}




RC appendEmptyBlock (SM_FileHandle *fHandle)
{
    RC returnvalue;	
    SM_PageHandle ph;

   ph = (char *) calloc(PAGE_SIZE, sizeof(char)); 

    returnvalue = fseek(fHandle->mgmtInfo,(fHandle->totalNumPages + 1)*PAGE_SIZE*sizeof(char) , SEEK_END); /*pointing the file pointer to the last page */

    if (returnvalue == RC_OK){
        fwrite(ph, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo); /* writes data from the memory block pointed by ph to the file i.e last page is filled with zero bytes. */
        fHandle->totalNumPages = fHandle->totalNumPages + 1;
        fHandle->curPagePos = fHandle->totalNumPages;
	rewind(fHandle->mgmtInfo);
	fprintf(fHandle->mgmtInfo, "%d\n" , fHandle->totalNumPages); /* updates total number of pages information in the file */
        fseek(fHandle->mgmtInfo, (fHandle->totalNumPages + 1)*PAGE_SIZE*sizeof(char), SEEK_SET);
        free(ph);
        return RC_OK;
	}
	else{
        free(ph);
		return RC_WRITE_FAILED;
	}
}

