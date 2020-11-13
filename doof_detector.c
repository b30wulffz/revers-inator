#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define CHUNK_SIZE 1000000

char *strrev(char *str, long long int len)
{
    char *a, *b;
    a = str;
    b = str + len - 1;
    char temp;
    while (a < b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
        a++;
        b--;
    }
    return str;
}

int checkDir(char *dirName)
{
    struct stat tmp;
    stat(dirName, &tmp);

    if (S_ISDIR(tmp.st_mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkFile(char *fileName)
{
    struct stat tmp;
    if (stat(fileName, &tmp) == 0 && fileName != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void progress(float perc)
{
    char prog[] = "........................................";
    int len = strlen(prog);
    int up = (int)((perc * len) / 100);
    for (int i = 0; i < up; i++)
    {
        prog[i] = '*';
    }
    char *tmpStr = (char *)malloc(sizeof(char) * 1000);
    int lenStr = 0;
    if (up != len)
    {
        lenStr = sprintf(tmpStr, "\r[ %s ] %0.2f%%", prog, perc);
    }
    else
    {
        lenStr = sprintf(tmpStr, "\r[ %s ] %0.2f%%\n\n", prog, perc);
    }
    write(1, tmpStr, lenStr);
    free(tmpStr);
}

char *getBaseName(char src[], char *dest)
{
    int len = strlen(src);
    if (src[len - 1] == '/')
    {
        len--;
    }
    int ind = len - 1;
    while (ind >= 0)
    {
        if (src[ind] == '/')
        {
            break;
        }
        ind--;
    }
    ind++;
    int size = len - ind;
    if (size > 0)
    {
        dest = (char *)malloc(sizeof(char) * size);
        int i = 0;
        while (ind < len)
        {
            dest[i] = src[ind];
            ind++;
            i++;
        }
    }
    else
    {
        dest = (char *)malloc(sizeof(char) * 1);
        dest[0] = '/';
    }
    return dest;
}

void printDetails(char *fullPath, char *name, int mode)
{
    char *tmpStr = (char *)malloc(sizeof(char) * 1000);
    int lenStr = 0;

    char *type = (char *)malloc(sizeof(char) * 20);

    switch (mode)
    {
    case 0:
        sprintf(type, "directory");
        break;

    case 1:
        sprintf(type, "old file");
        break;

    case 2:
        sprintf(type, "new file");
        break;

    default:
        sprintf(type, "file");
    }

    struct stat buffer;
    stat(fullPath, &buffer);

    lenStr = sprintf(tmpStr, "    * User has read permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IRUSR ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * User has write permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IWUSR ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * User has execute permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IXUSR ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * Group has read permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IRGRP ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * Group has write permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IWGRP ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * Group has execute permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IXGRP ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * Others have read permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IROTH ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * Others have write permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IWOTH ? "Yes" : "No");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "    * Others have execute permissions on %s '%s': %s\n", type, name, buffer.st_mode & S_IXOTH ? "Yes" : "No");
    write(1, tmpStr, lenStr);
}

int main(int argc, char *argv[])
{
    char *tmpStr = (char *)malloc(sizeof(char) * 1000);
    int lenStr = 0;

    if (argc == 4)
    {
        char *destFileName, *srcFileName, *dirName, *baseDestFileName, *baseSrcFileName, *baseDirName;
        destFileName = argv[argc - 3];
        srcFileName = argv[argc - 2];
        dirName = argv[argc - 1];
        baseDestFileName = getBaseName(argv[argc - 3], baseDestFileName);
        baseSrcFileName = getBaseName(argv[argc - 2], baseSrcFileName);
        baseDirName = getBaseName(argv[argc - 1], baseDirName);

        int missing = 0;

        lenStr = sprintf(tmpStr, "\n> Task 1: To check the permissions for the two files and the directory \n\n");
        write(1, tmpStr, lenStr);

        if (checkDir(dirName))
        {
            lenStr = sprintf(tmpStr, "> '%s' directory is created: Yes\n", baseDirName);
            write(1, tmpStr, lenStr);
            printDetails(dirName, baseDirName, 0);
        }
        else
        {
            lenStr = sprintf(tmpStr, "> '%s' directory is created: No\n", baseDirName);
            write(1, tmpStr, lenStr);
            lenStr = sprintf(tmpStr, "    * Hence further details regarding permissions could not be displayed\n");
            write(1, tmpStr, lenStr);
        }

        lenStr = sprintf(tmpStr, "\n############################################################\n\n");
        write(1, tmpStr, lenStr);

        if (checkFile(srcFileName))
        {
            lenStr = sprintf(tmpStr, "> Old file '%s' is created: Yes\n", baseSrcFileName);
            write(1, tmpStr, lenStr);
            printDetails(srcFileName, baseSrcFileName, 1);
            if (checkDir(srcFileName))
            {
                lenStr = sprintf(tmpStr, "    * [Error] Old file '%s' is a directory\n", baseSrcFileName);
                write(1, tmpStr, lenStr);
                missing = 2;
            }
        }
        else
        {
            lenStr = sprintf(tmpStr, "> Old file '%s' is created: No\n", baseSrcFileName);
            write(1, tmpStr, lenStr);
            lenStr = sprintf(tmpStr, "    * Hence further details regarding permissions could not be displayed\n");
            write(1, tmpStr, lenStr);
            missing = 1;
        }

        lenStr = sprintf(tmpStr, "\n############################################################\n\n");
        write(1, tmpStr, lenStr);

        if (checkFile(destFileName))
        {
            lenStr = sprintf(tmpStr, "> New file '%s' is created: Yes\n", baseDestFileName);
            write(1, tmpStr, lenStr);
            printDetails(destFileName, baseDestFileName, 2);
            if (checkDir(baseDestFileName))
            {
                lenStr = sprintf(tmpStr, "    * [Error] New file '%s' is a directory\n", baseDestFileName);
                write(1, tmpStr, lenStr);
                missing = 2;
            }
        }
        else
        {
            lenStr = sprintf(tmpStr, "> New file '%s' is created: No\n", baseDestFileName);
            write(1, tmpStr, lenStr);
            lenStr = sprintf(tmpStr, "    * Hence further details regarding permissions could not be displayed\n");
            write(1, tmpStr, lenStr);
            missing = 1;
        }

        lenStr = sprintf(tmpStr, "\n############################################################\n\n");
        write(1, tmpStr, lenStr);

        // Task 2

        lenStr = sprintf(tmpStr, "\n> Task 2: To check whether the contents in the new file are the reverse of the old file \n\n");
        write(1, tmpStr, lenStr);

        if (missing == 0)
        {
            int source_fd = open(srcFileName, O_RDONLY, 0);
            int dest_fd = open(destFileName, O_RDONLY, 0);
            int error = 0;
            if (source_fd == -1)
            {
                lenStr = sprintf(tmpStr, "> [Error] Old file '%s' cannot be accessed\n", baseSrcFileName);
                write(1, tmpStr, lenStr);
                error = 1;
            }
            if (dest_fd == -1)
            {
                lenStr = sprintf(tmpStr, "> [Error] New file '%s' cannot be accessed\n", baseDestFileName);
                write(1, tmpStr, lenStr);
                error = 1;
            }
            if (error != 1)
            {
                long long int length_f1 = lseek(source_fd, 0, SEEK_END);
                long long int length_f2 = lseek(dest_fd, 0, SEEK_END);

                lenStr = sprintf(tmpStr, "\n> Starting reverse comparison ... \n");
                write(1, tmpStr, lenStr);

                if (length_f1 == length_f2)
                {
                    if (length_f1 == 0)
                    {
                        lenStr = sprintf(tmpStr, "\n");
                        write(1, tmpStr, lenStr);
                        progress(100.0);
                        lenStr = sprintf(tmpStr, "> [Success] Files are same in reverse order\n");
                        write(1, tmpStr, lenStr);
                    }
                    else
                    {
                        long long int chunk = CHUNK_SIZE;
                        char *str1;
                        str1 = (char *)malloc(sizeof(char) * chunk);
                        char *str2;
                        str2 = (char *)malloc(sizeof(char) * chunk);

                        lenStr = sprintf(tmpStr, "> Taking chunk size: %lld Bytes\n", chunk);
                        write(1, tmpStr, lenStr);

                        lenStr = sprintf(tmpStr, "> [Hint] Increase chunk size for faster speed (upto 100000000 Bytes)\n\n");
                        write(1, tmpStr, lenStr);

                        long long int x = length_f1;
                        int fail = 0;

                        lseek(source_fd, 0, SEEK_SET);
                        progress(0.0);
                        do
                        {
                            long long int diff = x - chunk;
                            if (diff <= 0)
                            {
                                chunk = x;
                                x = 0;
                            }
                            else
                            {
                                x = diff;
                            }

                            float perc = ((length_f1 - x) / (length_f1 * 1.0)) * 100;

                            read(source_fd, str1, chunk);

                            lseek(dest_fd, x, SEEK_SET);
                            read(dest_fd, str2, chunk);
                            str2 = strrev(str2, chunk);

                            if (strcmp(str1, str2) != 0)
                            {
                                progress(100.0);
                                lenStr = sprintf(tmpStr, "> Files are not same in reverse order\n");
                                write(1, tmpStr, lenStr);
                                fail = 1;
                                break;
                            }
                            progress(perc);

                        } while (x > 0);

                        if (fail == 0)
                        {
                            lenStr = sprintf(tmpStr, "> [Success] Files are same in reverse order\n");
                            write(1, tmpStr, lenStr);
                        }
                    }
                }
                else
                {
                    lenStr = sprintf(tmpStr, "\n");
                    write(1, tmpStr, lenStr);
                    progress(100.0);
                    lenStr = sprintf(tmpStr, "> Files are not same in reverse order\n");
                    write(1, tmpStr, lenStr);
                }
                lenStr = sprintf(tmpStr, "> Comparison completed successfully\n");
                write(1, tmpStr, lenStr);
            }
            else
            {
                lenStr = sprintf(tmpStr, "> Comparison operation could not be performed\n");
                write(1, tmpStr, lenStr);
            }
        }
        else if (missing == 1)
        {
            lenStr = sprintf(tmpStr, "> [Error] Atleast one of the two files are missing\n");
            write(1, tmpStr, lenStr);
            lenStr = sprintf(tmpStr, "> Comparison operation could not be performed\n");
            write(1, tmpStr, lenStr);
        }
        else
        {
            lenStr = sprintf(tmpStr, "> [Error] Atleast one of the two files is a directory\n");
            write(1, tmpStr, lenStr);
            lenStr = sprintf(tmpStr, "> Comparison operation could not be performed\n");
            write(1, tmpStr, lenStr);
        }
    }
    else
    {
        lenStr = sprintf(tmpStr, "> [Error] Sufficient arguments not supplied\n");
        write(1, tmpStr, lenStr);
    }

    lenStr = sprintf(tmpStr, "\n############################################################\n\n");
    write(1, tmpStr, lenStr);

    lenStr = sprintf(tmpStr, "# Developed By: Shlok Pandey\n");
    write(1, tmpStr, lenStr);
    lenStr = sprintf(tmpStr, "# Github: @b30wulffz\n");
    write(1, tmpStr, lenStr);

    lenStr = sprintf(tmpStr, "\n------------------------------------------------------------\n\n");
    write(1, tmpStr, lenStr);

    free(tmpStr);
    return 0;
}