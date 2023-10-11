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
     *  runs the program that mimics ls -n with one cmd line input
     */

    if (argc != 2) 
    {
        myWrite("Usage: ./run <directory>\n");
        return 1;
    }

    char *path = argv[1];
    int file_fd;
    struct stat file_info; //gets intital stat
    struct stat st; //gets stat on files to write to screen

    file_fd = myOpen(path, O_RDONLY);

    //error checks
    if (file_fd == -1)
    {
        myWrite("Error opening directory\n");
        return 1;

    }
    if(myStat(path, &file_info) == -1)
    {
        myWrite("Stat error\n");
        myClose(file_fd);
        return 1;
    }

    //store file names to sort alphabetically later
    char *filenames[BUFSIZ];
    char *filenamesShort[BUFSIZ];
    int numFiles = 0; 
    // if directory open it to get handle, and use getdents to get info
    if (S_ISDIR(file_info.st_mode)) 
    {
        // open dir and verify
        int dir_fd = myOpen(path, O_RDONLY | O_DIRECTORY);

        if(dir_fd == -1)
        {
            
            myWrite("Error opening directory\n");
            return 1;
        }

        // Make repeated calls to getdents to get a list of filenames inside the directory.
        char buffer[BUFSIZ];
        while(1)
        {
            //call getdents
            int bytes = myGetDents(dir_fd, buffer);

            //error case
            if(bytes == -1)
            {
                myWrite("Getdents error\n");
                break;
            }

            //end of directory
            if(bytes == 0) break;

            //get file info
            struct dirent *d;
            for(int i = 0; i < bytes;)
            {
                d = (struct dirent *)(buffer + i);
                char * name = d->d_name;
                filenamesShort[numFiles] = myStrdup(name);

                //skips hidden files
                if(name[0] != '.')
                {
                    
                    // error checking
                    if (myStat(name, &st) == -1) {
                        myWrite("Stat Error");
                        return 1;
                    }
                    
                    // Store filenames in the array
                    char fullPath[BUFSIZ] = "";
                    char *pathFormat[] = {path, "/", name};
                    size_t len = sizeof(pathFormat) / sizeof(pathFormat[0]);

                    concatAll(fullPath, pathFormat, len);
        
                    filenames[numFiles] = myStrdup(fullPath);
                    numFiles++;
                }

                i += d->d_reclen;
            }
        }
        myClose(dir_fd);
    }
    else if (S_ISREG(file_info.st_mode)) //if file, write its info in same manner as above
    {
          
        // Get the file information for the specified file
        if (myStat(path, &st) == -1) {
            myWrite("Stat Error");
            return 1;
        }
        filenames[numFiles++] = myStrdup(path); 
    }

    // Sort the filenames alphabetically
    mySort(filenames, numFiles);
    mySort(filenamesShort, numFiles);

    // After sorting input, loop through the filenames and call formatOutput
    for (int i = 0; i < numFiles; i++) 
    {
        // Get the file information for the specified file
        if (myStat(filenames[i], &st) == -1) {
            myWrite("Stat Error");
            return 1;
        }
        
        formatOutputls(st, filenamesShort[i]);
        free(filenames[i]); // Free allocated memory for filename
        free(filenamesShort[i]);
    }
    
    return 0;
}
