#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>

#include <stdio.h>
#include <string.h>
#include "lib.h"
void formatOutput(struct stat st, char *name)
{

    char info[256] = "";
    char timestamp[20] = "";
    char permissions[11]; // To store the permissions string
    /*
    printf("%s\n", st.st_mode);
    // Extract file permissions and format them
    char * permissionsFormatted[] ={
             (st.st_mode & S_IRUSR) ? 'r' : '-',
             (st.st_mode & S_IWUSR) ? 'w' : '-',
             (st.st_mode & S_IXUSR) ? 'x' : '-',
             (st.st_mode & S_IRGRP) ? 'r' : '-',
             (st.st_mode & S_IWGRP) ? 'w' : '-',
             (st.st_mode & S_IXGRP) ? 'x' : '-',
             (st.st_mode & S_IROTH) ? 'r' : '-',
             (st.st_mode & S_IWOTH) ? 'w' : '-',
             (st.st_mode & S_IXOTH) ? 'x' : '-',
             (st.st_mode & S_ISUID) ? 's' : '-',
             (st.st_mode & S_ISGID) ? 's' : '-'
    };
    size_t length = sizeof(permissionsFormatted) / sizeof(permissionsFormatted[0]);
    concatAll(permissions, permissionsFormatted, length);
    */
    struct tm * tm = localtime(&st.st_mtime);

    char * timestampFormat[] = {
        convertIntToMonth(tm->tm_mon), " ",
        (char*)intToString(tm->tm_mday), " ",
        (char*)intToString(tm->tm_hour), ":",
        (char*)intToString(tm->tm_min)
    };

    // Calculate the length of the array
    size_t length = sizeof(timestampFormat) / sizeof(timestampFormat[0]);

    concatAll(timestamp, timestampFormat, length);
    char *infoFormatted[] = {
        //permissions, " ",
        (char*)intToString((int)st.st_uid), " ",
        (char*)intToString((int)st.st_gid), " ",
        (char*)intToString((int)st.st_size), " ",
        timestamp, " ",
        name, "\n"
    };

    length = sizeof(infoFormatted) / sizeof(infoFormatted[0]);
    concatAll(info, infoFormatted, length);
    myWrite(info);

}
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
                    if (myStat(name, &st) == -1) {
                        myWrite("Stat Error");
                        return 1;
                    }
                    formatOutput(st, name);
                }

                i += d->d_reclen;
            }
        }
        myClose(dir_fd);
    }
    //if file, write its info
    else if (S_ISREG(file_info.st_mode)) 
    {
        struct stat st;
        char info[256];
        char timestamp[20];
        

        // Get the file information for the specified file
        if (myStat(path, &st) == -1) {
            perror("myStat");
            return 1;
        }
        formatOutput(st, path);
    }
    return 0;
}

