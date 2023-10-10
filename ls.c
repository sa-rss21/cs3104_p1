#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib.h"


void formatOutput(struct stat st, char *name)
{
    /* 
     *  This function formats a stat struct
     *  into an ls -n format 
     *
     *  'file-type - permissions - links - timestamp - owner - file size - date - filename' 
     *
     */

    char info[BUFSIZ] = ""; // stores all file info to write to screen 
    char timestamp[21] = ""; // stores custom timestamp
    char permissions[11] = ""; // To store the permissions string
    
    // Extract file permissions and format them
    char * permissionsFormatted[] = {
             (S_ISDIR(st.st_mode)) ? "d" : "-",
             (st.st_mode & S_IRUSR) ? "r" : "-",
             (st.st_mode & S_IWUSR) ? "w" : "-",
             (st.st_mode & S_IXUSR) ? "x" : "-",
             (st.st_mode & S_IRGRP) ? "r" : "-",
             (st.st_mode & S_IWGRP) ? "w" : "-",
             (st.st_mode & S_IXGRP) ? "x" : "-",
             (st.st_mode & S_IROTH) ? "r" : "-",
             (st.st_mode & S_IWOTH) ? "w" : "-",
             (st.st_mode & S_IXOTH) ? "x" : "-", };
    
    //concat permissions 
    size_t len = sizeof(permissionsFormatted) / sizeof(permissionsFormatted[0]);
    concatAll(permissions, permissionsFormatted, len);
    
    struct tm * tm = localtime(&st.st_mtime);

    // Extract time settings
    char * timestampFormat[] = {
        convertIntToMonth(tm->tm_mon), " ",
        intToString(tm->tm_mday), " ",
        intToString(tm->tm_hour), ":",
        intToString(tm->tm_min)
    };

    //concat timestamp
    len= sizeof(timestampFormat) / sizeof(timestampFormat[0]);
    concatAll(timestamp, timestampFormat, len);

    
    //Format all data
    char *infoFormatted[] = {
        permissions, " ",
        intToString((int)st.st_nlink), " ",
        intToString((int)st.st_uid), " ",
        intToString((int)st.st_gid), " ",
        intToString((int)st.st_size), " ",
        timestamp, " ",
        name, 
        (S_ISDIR(st.st_mode)) ? "/" : "",
        "\n"
  };

    //concat all data
    len= sizeof(infoFormatted) / sizeof(infoFormatted[0]);
    concatAll(info, infoFormatted, len);
    myWrite(info);

}

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
                    filenames[numFiles++] = myStrdup(fullPath); 
            

                    
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

    // After sorting input, loop through the filenames and call formatOutput
    for (int i = 0; i < numFiles; i++) 
    {
        

        // Get the file information for the specified file
        if (myStat(filenames[i], &st) == -1) {
            myWrite("Stat Error");
            return 1;
        }
        
        formatOutput(st, filenames[i]);
        free(filenames[i]); // Free allocated memory for filename
        
    }

    return 0;
}
