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
     *  runs the program that mimics cp by copying contents of one file to another,
     *  creating a new file if neccessary
     */

    char buffer[BUFSIZ];
    int fd1, fd2, n;

    if (argc != 3) 
    {
        myWrite("Usage: ./mycp file1 file2\n");
        return 1;
    }

    //open source
    fd1 = myOpen(argv[1], O_RDONLY);
    if(fd1 == -1)
    {
        myWrite("Error opening file 1\n");
        return 1;
    }

    //open destination
    fd2 = myOpen(argv[2], O_CREAT | O_WRONLY | O_TRUNC);
    if(fd2 == -1)
    {
        //if cant open, try creating one and then opening again
        if(touch(argv[2]) != 0)
        {
            myWrite("Failed to create file\n");
            return 1;
        }

        fd2 = myOpen(argv[2], O_CREAT | O_WRONLY | O_TRUNC);
        if(fd2 == -1)
        {
            myWrite("Failed to open file2 so tried to create one and it still failed\n");
            myClose(fd1);
            return 1;
        }
    }
    
    //write contents of source to destination
    while ((n = myRead(fd1, buffer, sizeof(buffer))) > 0) {
        if (myWriteFile(fd2, buffer, n) != n) {
            myWrite("Error writing to destination file\n");
            myClose(fd1);
            myClose(fd2);
            return 1;
        }
    }
    
    //close files and exit
    myClose(fd2);
    myClose(fd1);

    return 0;

}
