#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../lib.h"

int main(int argc, char *argv[]) 
{
    /*
     *
     *  runs the program that mimics rm by copying calling the
     *  unlink system call on all the command line args
     *
     */

    if (argc < 2)
    {
        myWrite("Usage: ./myrm  <file1> <file2> ... <fileN>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (myUnlink(argv[i]) == -1)
        {
            myWrite("Error deleting file\n");
            return 1;
        }
    }

    return 0;

}
