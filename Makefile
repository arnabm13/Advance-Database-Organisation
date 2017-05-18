#GCC is our compiler
CC=gcc
#test_assign2_1 is the target file.
all:test_assign3_1
test_assign3_1: test_assign3_1.o dberror.o storage_mgr.o buffer_mgr.o buffer_mgr_stat.o expr.o rm_serializer.o record_mgr.o
	$(CC) test_assign3_1.o dberror.o storage_mgr.o buffer_mgr.o buffer_mgr_stat.o expr.o record_mgr.o -lm rm_serializer.o -o test_assign3_1
dberror.o: dberror.c dberror.h
	$(CC) -c dberror.c
storage_mgr.o: storage_mgr.c storage_mgr.h
	$(CC) -c storage_mgr.c
buffer_mgr.o: buffer_mgr.c buffer_mgr.h dberror.h storage_mgr.h
	$(CC) -c buffer_mgr.c
buffer_mgr_stat.o: buffer_mgr_stat.c buffer_mgr.h dberror.h storage_mgr.h buffer_mgr_stat.h
	$(CC) -c buffer_mgr_stat.c
expr.o: expr.c dberror.h record_mgr.h expr.h tables.h
	$(CC) -c expr.c
rm_serializer.o: rm_serializer.c dberror.h record_mgr.h expr.h tables.h
	$(CC) -c rm_serializer.c
record_mgr.o: record_mgr.c buffer_mgr.h storage_mgr.h record_mgr.h expr.h tables.h
clean :
	rm dberror.o test_assign3_1.o storage_mgr.o buffer_mgr.o buffer_mgr_stat.o expr.o rm_serializer.o record_mgr.o expr.c
