ID No  :  A20351185
NAME   : Bijay Sharma 
EMAIL  : bsharma@hawk.iit.edu

ID No  :  A20353463
NAME   : Arnab Mukhopadhyay
EMAIL  :  amukhopa@hawk.iit.edu 

ID No  :  A20345760
NAME   : Ahad Puthawala 
EMAIL  :  aputhawa@hawk.iit.edu

FILES IN THE DIRECTORY
--------------------------------------
dberror.c
dberror.o		: 
dberror.h 	: 
storage_managerfunction.h
Storage_mgr.h
test_assign_1.c
test_assign_1
test_helper.h

Input file: 
------------
There is no input file. We will use test_assign1_1.c as test script.

Output file:
-------------
Filename.txt


Process to Compilate and run:
----------------------------------------------------
 To compile: gcc dberror.c test_assign1_1.c -o test_assign_1    //this will create sotoragemangerfile which we will run                  
 To run    : ./test_assign_1

Details:
-------------
1.test_assign_1.c -> This is the test script and .c file containing the main function of the program. This contains storage_managerfunction.h header file.
2.Storage_managerfunction.h -> This file contains the implementation of the functions. It contains dberror.h,test_helper.h and storage_mgr.h.
3.Storage_mgr.h -> This function contains the declaration of all the functions of the program which are implemented.
4.dberror.h -> It contains all the error code returned by anyfunction as well as the definition of the structure of the file containing the informations like current
position of the pointer, file name etc.
5.dberror.c-> It is another c file used in the program which contains the definition of few error handling functions.
6.test_helper.h -> This file is included in the main file and contains the definitions of all Test functions.

