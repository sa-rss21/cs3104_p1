#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "../lib.h"

int main(int argc, char *argv[]) 
{
    /*
     *  runs the program that mimics cp
     */

    if (argc != 2) 
    {
        myWrite("Usage: ./run <directory>\n");
        return 1;
    }

    return 0;
}
