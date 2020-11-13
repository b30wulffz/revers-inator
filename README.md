# ReadMe - Reversinator and Doof-Detector

## For Reversinator:

This tool reverses the file (of upto 10^18 bytes) for no reason at all.
Now you can reverse confidential files and delete original. 
Not at all evil. HaHaHa.

To compile: ```gcc reversinator.c -o reversinator```

Input format: ```./reversinator sourceFile```
Example: ```./reversinator ./dummyFile.txt```

Important points to note before running the program:
 - Default chunk size is set to 10^6 bytes which takes around 49 seconds to reverse a 1GB file.
 - Increasing the chunk size based on available memory would make the program run faster.
 - This program is tested with chunk size upto 10^8 bytes, which took around 8 seconds to reverse a 1GB file.
 - To increase chunk size, update the macro CHUNK_SIZE with a most suitable value(in bytes).
    - Example: ```#define CHUNK_SIZE 100000000```
 - Increasing chunk size with a value more than available memory would crash the program.
 - To change output directory, update the macro DIR_NAME to the value of your choice.
    - Example: ```#define DIR_NAME "./DoofsEvilInc"```
 - Fun observations with huge files: The program took around 5 minutes 25 seconds to reverse a 8 GB file with 10^6 bytes chunk size.
 - Time Complexity: O(n)

Some assumptions:
 - Relative path for the source file can also be supplied.
 - Argument count should be equal to one.
 - Input file should be a text file with characters having ascii values in the range of char(1 byte) data type.
 - By default, the directory(if does not exists) and the new file is created with 777 permissions.


## For Doof-Detector:

This tool checks whether Reversinator was successful in executing its plans.
That is, it verifies whether original file and generated file are reverse of each other, and prints the permissions of new file, old file and directory supplied.

To compile: ```gcc doof_detector.c -o doof_detector```

Input format: ```./doof_detector newFile oldFile directory```
Example: ```./doof_detector ./DoofsEvilInc/dummyFile.txt ./dummyFile.txt ./DoofsEvilInc/```

Important points to note before running the program:
 - Default chunk size is set to 10^6 bytes which takes around 8-9 seconds to compare two 1GB files.
 - Increasing the chunk size based on available memory would make the program run faster.
 - This program is tested with chunk size upto 10^7 bytes, which took around 7 seconds to compare two 1GB files.
 - To increase chunk size, update the macro CHUNK_SIZE upto most suitable value(in bytes).
   - Example: ```#define CHUNK_SIZE 10000000```
 - Increasing chunk size with a value more than available memory would crash the program.
 - Fun observations with huge files: The program took around 8 minutes 14 seconds to compare two 8 GB files with 10^6 bytes chunk size.
 - Time Complexity: O(n)

Some assumptions:
 - If folder name is supplied in the position of old or new file name, it's permissions will be displayed, but the files cannot be compared.
 - New file, old file and directory are independent of each other. 
 - The order of displaying permissions is as follows: Directory, Old File, New File.
 - Relative path for the old file, new file and directory can also be supplied.
 - Argument count should be equal to four.
 - Both input files should be text files with characters having ascii values in the range of char(1 byte) data type.

## Machine Specifications
 - **Operating System:** 64 bit Windows 10 Home running WSL 2 
 - **Terminal:** Microsoft Windows Subsystem for Linux Launcher
 - **Processor:** Intel Core i7-8750H CPU @ 2.20 GHz 2.21 GHz
 - **RAM:** 16 GB

Agent P!
Oh wait, what?