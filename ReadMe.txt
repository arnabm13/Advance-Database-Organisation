*****************************************************Record Manager***************************************************************
----------------------------------------------------------------------------------------------------------------------------------
ID No  :  A20351185
NAME   : Bijay Sharma 
EMAIL  : bsharma@hawk.iit.edu

ID No  :  A20353463
NAME   : Arnab Mukhopadhyay
EMAIL  :  amukhopa@hawk.iit.edu 

ID No  :  A20345760
NAME   : Ahad Puthawala 
EMAIL  :  aputhawa@hawk.iit.edu
__________________________________________________________________________________________________________________________________

Porblem Statement : The objective of the project is to make a project that will implement Record Manager. The record manager helps a client
to create a table, insert records, update record, delete record and also finds the out put of a query given a condition. The table is stored in a table in a file with the help of storage manager. This project also takes help of buffer manager for better memory management.

--------------------------------------------------------------------------------------------------------------------------------------

How to run the project?

Step 1: Run makefile command
Step 2: Run ./test_assign3_1




FILES IN THE DIRECTORY
----------------------
record_mgr.c
record_mgr.h
expr.c
expr.h
tables.h
rm_serializer.c
buffer_mgr.c
buffer_mgr.h
buffer_mgr_stat.c
buffer_mgr_stat.h
storage_mgr.c
Storage_mgr.h
test_assign2_1.c
test_helper.h
------------------------
Functions and their details:

1. initRecordManager: Function used to initialize a record Manager.

2. shutdownRecordManager : This function is used to shutdown the record manager.

3. createTable : This function helps to create a table with a given name. This is the function for table handling. It first checks if the table is already in the system. Basically, it checks a file with same name of the table. The table actually holds the detals. Information of tables is spreaded as follows in the file.
0th Page---> This page contains the number of pages in a file.
1st page---> This page contains information about how the table will be spreaded on the file. This contains the size of the schema, size of the slot. Number of maximum of slots in a page, number of tuples in the table, the number of the page from where the record starts.
2nd Page---> This page contains the schema of the table.
3rd Page---> This page contains the records.

3.open table: After creation of the first table, this is the second function which helps record manager. It initializes the buffer Manager and page handler to hold the information of a page. It pins the 1st page and 2nd page of the file to the buffer. It means it loads tableinfo (information how the table will be spreaded on the file) and schema information to the buffer. The buffer will follow FIFO strategy.

4.closeTable: This function closes the file and bufferManager. 

5.deleteTable: It delets the table and the file created for it.

6.insertRecord : This function basically helps to do all the operation related to insertion of a record in a table. It first find if there is any tstone available in the list. if there is any, record attributes are updated accordingly.If there is no any RID available. Then it finds available empty slot and insert the record in that slot. If the total number of slots of a page is occupied then a new page is created to hold the records.  In insertRecord, there is serializedRecord function to get record in proper format. Along with serilialized function, it will also use pin,unpin,markdirty function of the buffer manager for memory management.

7. deleteRecord: This function is used to delete record from a table. It first checks if the tstone list is empty. if tstone is found empty then it new tNode, updates its content with the values from RID and adds to the tstone list. In case the tstone is not empty, then tstone_iter is used to go to the end of the list  and a new tNode and finally number of tumples is reduced and final data is written to the file.

8. updateRecord: This function update any record exisiting in the table already.

9. getRecord: This function helps to get record from table using an RID. Beforing try to fetch the record, it first checks if the RID exists.	Then it finally fetches the page and pins it in the buffer.

10. StartScan: This function initializes RM_ScanHandle data structure. It also defines a node storing the information about the record to be searched and the condition to be evaluated. 

11. Next: This function gets a record with the help of page number and slot number. It also checks tombstone for deleted ids. In case of a deleted record, it first check if the slot is the last in the page, if not then it uses this slot or else it creates a new page.

12. Closescan : This frees the scan handler.

13. createSchema: It creates the schema.

14. freeSchema: It frees the memory allocated to all the schema variables.

 






