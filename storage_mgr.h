/*
 * dberror.h
 *
 *  Created on: Sep 6, 2015
 *      Author: Arnab
 */

#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include "dberror.h"

typedef struct SM_FileHandle
{
  char *fileName;
  int totalNumPages;
  int curPagePos;
  void *mgmtInfo;

} SM_FileHandle;


typedef char* SM_PageHandle;

extern void initStorageManager (void);

extern RC createPageFile (char *fileName);

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle);

extern RC closePageFile (SM_FileHandle *fHandle);
extern RC readBlock (int , SM_FileHandle *, SM_PageHandle);

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);

extern RC destroyPageFile (char *fileName);

int getBlockPos (SM_FileHandle *fHandle);

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

#endif
