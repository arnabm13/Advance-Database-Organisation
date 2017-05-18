#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H
#include "dberror.h"
#include "storage_mgr.h"
#include "stdlib.h"


// Replacement Strategies
typedef enum ReplacementStrategy {
  RS_FIFO = 0,
  RS_LRU = 1,
  RS_CLOCK = 2,
  RS_LFU = 3,
  RS_LRU_K = 4
} ReplacementStrategy;

// Data Types and Structures
typedef int PageNumber;
#define NO_PAGE -1

typedef short bool;
#define TRUE 1
#define FALSE 0

typedef struct BM_BufferPool {
  char *pageFile;
  int numPages;
  ReplacementStrategy strategy;
  void *dataHandler; // use this one to store the bookkeeping info your buffer
                  // manager needs for a buffer pool
} BM_BufferPool;

typedef struct BM_PageHandle {
  PageNumber pageNum;
  char *data;
} BM_PageHandle;

// Per Buffer Pool frame details
typedef struct BM_Pool_Page_Details {
    bool isDirty;
    int pfCount;
    PageNumber pn;
    bool isReplaceable;
    struct LRU_LinkEntry *linkEntry;
    char data[PAGE_SIZE];
} BM_Pool_Page_Details;

typedef struct BM_Pool_VPTable {
    int deleteFlag;
    void* entry[256];
} BM_Pool_VPTable;


/**
Defining the Linked List Structure
**/
typedef struct LinkEntry {
  BM_Pool_Page_Details *frame;
  struct LinkEntry *next;
  struct LinkEntry *prev;
} LinkEntry;

typedef struct BM_Pool_Replace_Data {
     LinkEntry *head; // this points to the start of the list
     LinkEntry *tail; // this points to end of the list initially head and tail points to null
     int trailingFrame;
     int currFrame;

} BM_Pool_Replace_Data;


typedef struct PoolManager {
    BM_Pool_Replace_Data strategyInfo;
    SM_FileHandle fhandler;
    BM_Pool_Page_Details *bm_pool_arr;
    BM_Pool_VPTable pageFrame_pointer;
    int readCount;
    int writeCount;
} PoolManager;

#define MAKE_POOL()				\
  ((BM_BufferPool *) malloc (sizeof(BM_BufferPool)))

#define MAKE_PAGE_HANDLE()		\
  ((BM_PageHandle *) malloc (sizeof(BM_PageHandle)))

// Buffer Manager Interface - Pool Handling
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
		  const int numPages, ReplacementStrategy strategy,
		  void *stratData);
RC shutdownBufferPool(BM_BufferPool *const bm);
RC forceFlushPool(BM_BufferPool *const bm);

// Buffer Manager Interface - Access Pages
RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page);
RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page);
RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page);
RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
	    const PageNumber pageNum);

// Statistics Interface
PageNumber *getFrameContents (BM_BufferPool *const bm);
bool *getDirtyFlags (BM_BufferPool *const bm);
int *getFixCounts (BM_BufferPool *const bm);
int getNumReadIO (BM_BufferPool *const bm);
int getNumWriteIO (BM_BufferPool *const bm);

#endif