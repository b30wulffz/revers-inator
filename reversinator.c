#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define CHUNK_SIZE 1000000
#define DIR_NAME "./DoofsEvilInc"

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
    if (stat(fileName, &tmp) == 0)
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

int main(int argc, char *argv[])
{
    char *tmpStr = (char *)malloc(sizeof(char) * 1000);
    int lenStr = 0;
    char dirName[] = DIR_NAME;
    char *baseDirName = getBaseName(dirName, baseDirName);

    lenStr = sprintf(tmpStr, "\n> Welcome to Reversinator - A (non-evil) File Reversal Program\n\n");
    write(1, tmpStr, lenStr);

    if (argc == 2)
    {
        char *srcFileName, *destFileName;
        srcFileName = argv[argc - 1];
        destFileName = getBaseName(argv[argc - 1], destFileName);
        if (checkFile(srcFileName))
        {
            if (!checkDir(srcFileName))
            {

                int source_fd = open(srcFileName, O_RDONLY, 0);
                if (source_fd != -1)
                {
                    if (checkDir(dirName))
                    {
                        lenStr = sprintf(tmpStr, "> '%s' directory found!\n", baseDirName);
                        write(1, tmpStr, lenStr);
                    }
                    else
                    {
                        lenStr = sprintf(tmpStr, "> Directory does not exists! Creating '%s'\n", baseDirName);
                        write(1, tmpStr, lenStr);

                        mkdir(dirName, 0777);
                    }

                    lenStr = sprintf(tmpStr, "> Creating '%s' inside '%s' directory\n", destFileName, baseDirName);
                    write(1, tmpStr, lenStr);

                    char *dest = (char *)malloc(sizeof(char) * 200);
                    sprintf(dest, "%s/%s", dirName, destFileName);
                    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0777);

                    if (dest_fd != -1)
                    {

                        lenStr = sprintf(tmpStr, "> '%s/%s' created successfully\n", dirName, destFileName);
                        write(1, tmpStr, lenStr);

                        long long int length = lseek(source_fd, 0, SEEK_END);

                        if (length > 0)
                        {

                            long long int chunk = CHUNK_SIZE;
                            char *char_str;
                            char_str = (char *)malloc(sizeof(char) * chunk);

                            long long int x = length;

                            lenStr = sprintf(tmpStr, "\n> Starting reversal process ... \n");
                            write(1, tmpStr, lenStr);

                            lenStr = sprintf(tmpStr, "> Taking chunk size: %lld Bytes\n", chunk);
                            write(1, tmpStr, lenStr);

                            lenStr = sprintf(tmpStr, "> [Hint] Increase chunk size for faster speed (upto 100000000 Bytes)\n\n");
                            write(1, tmpStr, lenStr);

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

                                float perc = ((length - x) / (length * 1.0)) * 100;

                                lseek(source_fd, x, SEEK_SET);
                                read(source_fd, char_str, chunk);
                                char_str = strrev(char_str, chunk);
                                write(dest_fd, char_str, chunk);

                                progress(perc);
                            } while (x > 0);
                            if (x != 0)
                            {
                                lenStr = sprintf(tmpStr, "> [Error] Source file '%s' can not be reversed completely\n", srcFileName);
                                write(1, tmpStr, lenStr);
                            }
                            else
                            {
                                lenStr = sprintf(tmpStr, "> Reversal completed successfully\n");
                                write(1, tmpStr, lenStr);
                            }
                        }
                        else
                        {
                            lenStr = sprintf(tmpStr, "> [Error] Source file '%s' is empty\n", srcFileName);
                            write(1, tmpStr, lenStr);
                        }
                    }
                    else
                    {
                        lenStr = sprintf(tmpStr, "> [Error] Destination file '%s' cannot be accessed inside directory '%s'\n", destFileName, baseDirName);
                        write(1, tmpStr, lenStr);
                    }
                    close(source_fd);
                    close(dest_fd);
                }
                else
                {
                    lenStr = sprintf(tmpStr, "> [Error] Source file '%s' cannot be accessed\n", srcFileName);
                    write(1, tmpStr, lenStr);
                }
            }
            else
            {
                lenStr = sprintf(tmpStr, "> [Error] Source file '%s' is a directory\n", srcFileName);
                write(1, tmpStr, lenStr);
            }
        }
        else
        {
            lenStr = sprintf(tmpStr, "> [Error] Source file '%s' not found\n", srcFileName);
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