#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>

#include <stdio.h>
#include <string.h>
#include "lib.h"

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        myWrite("Usage: ./ <directory>\n");

        return 1;
    }

    char *path = argv[1];
    int file_fd;
    struct stat file_info;

    file_fd = myOpen(path, O_RDONLY);
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
    // if directory open it to get handle, and use getdents to get info
    if (S_ISDIR(file_info.st_mode)) 
    {
        // open directory
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
                    struct stat st;
                    char info[256] = "";
                    char timestamp[20] = "";
                    

                    // Get the file information for the entry
                    if (myStat(name, &st) == -1) {
                        myWrite("Stat Error");
                        return 1;
                    }
                    
                    struct tm * tm = localtime(&st.st_mtime);

                    //attach day 
                    myStrcat(timestamp, (char*)intToString(tm->tm_mday));
                    myStrcat(timestamp, " ");
                    //attach month
                    myStrcat(timestamp, (char *)convertIntToMonth(tm->tm_mon));
                    myStrcat(timestamp, " ");
                    //attach hour
                    myStrcat(timestamp, (char*)intToString(tm->tm_hour));
                    myStrcat(timestamp, ":");
                    //attach minute
                    myStrcat(timestamp, (char*)intToString(tm->tm_min));
                                        //concat all
                    myStrcat(info, (char*)intToString((int)st.st_uid));
                    myStrcat(info, " ");
                    myStrcat(info, (char*)intToString((int)st.st_gid));
                    myStrcat(info, " ");
                    myStrcat(info, (char*)intToString((int)st.st_size));
                    myStrcat(info, " ");
                    myStrcat(info, timestamp);
                    myStrcat(info, " ");
                    myStrcat(info, name);
                    myStrcat(info, "\n");

                    myWrite(info);
                    
                }

                i += d->d_reclen;
            }
        }
        myClose(dir_fd);
    }
    //if file, write its info
    else if (S_ISREG(file_info.st_mode)) {
        struct stat st;
        char info[256];
        char timestamp[20];
        

        // Get the file information for the specified file
        if (myStat(path, &st) == -1) {
            perror("myStat");
            return 1;
        }
        // Format the truncated timestamp
        strftime(timestamp, sizeof(timestamp), "%b %d %Y %H:%M:%S", localtime(&st.st_mtime));

        // Print the numeric user and group IDs along with other file information
        snprintf(info, sizeof(info), "%5ld %5ld %8lld %s %s", (long)st.st_uid, (long)st.st_gid, (long long)st.st_size, timestamp, path);
        myWrite(info);
    }
    return 0;
}

