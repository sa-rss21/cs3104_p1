#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <fcntl.h>
#include <unistd.h>
#include "../lib.h"

int main(int argc, char *argv[]) 
{
    /*
     *  This program mimics the touch command by creating a file
     */

    if (argc < 2) 
    {
        myWrite("Usage: ./mytouch <file1> [file2] ...\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) 
        //call the touch command from library.c 
        //(its in there because it needs to be accessed by cp.c as well)
        touch(argv[i]);

    return 0;
}

