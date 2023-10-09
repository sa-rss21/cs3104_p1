#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

#define WRITE_SYSCALL 1
#define GETDENTS_SYSCALL 217
#define OPEN_SYSCALL 2
#define CLOSE_SYSCALL 3
#define STAT_SYSCALL 4
size_t myStrlen(char * str)
{
    size_t i= 0;
    while(*str != '\0')
    {
        i++;
        str++;
    }
    return i;
}

int myWrite(char * text)
{
    long handle = 1;     // 1 for stdout, 2 for stderr, file handle from open() for files
    int ret = -1;       // Return value received from the system call
    size_t len = myStrlen(text);

    //uses inline with shortcuts to write to screen
    asm("syscall" : "=a" (ret) : "0"(WRITE_SYSCALL), "D"(handle), "S"(text), "d"(len) : "cc", "rcx", "r11", "memory");
    return ret;
}
int myOpen(char * path, mode_t mode)
{
    int ret;
    asm("syscall" 
            : "=a"(ret)
            : "0"(OPEN_SYSCALL), "D"(path), "S"(mode), "d"(S_IRUSR | S_IWUSR)
            : "cc", "rcx", "r11", "memory");
    return ret;
    //return syscall(OPEN_SYSCALL, path, mode, S_IRUSR | S_IWUSR);
}
int myStat(const char * path, struct stat * statbuf)
{
    int ret;
    asm("syscall" 
            : "=a" (ret) 
            : "0"(STAT_SYSCALL), "D"(path), "S"(statbuf) 
            : "cc", "rcx", "r11", "memory");

    return ret;
    
}
int myClose(int handle)
{
    int ret;

    // Using inline assembler with shortcuts.
    asm("syscall" 
            : "=a" (ret) 
            : "0"(CLOSE_SYSCALL), "D"(handle) 
            : "cc", "rcx", "r11", "memory");
    return ret;

}
int myGetDents(int fd, char * buffer)
{
    int ret;
    asm("syscall"
            : "=a" (ret)
            : "0"(GETDENTS_SYSCALL), "D"(fd), "S"(buffer), "d"(BUFSIZ)
            : "cc", "rcx", "r11", "memory");
    return ret;
    return syscall(GETDENTS_SYSCALL, fd, buffer, BUFSIZ);
}
int myStrcmp(char * str1, char * str2)
{

    while(*str1 != '\0' && *str2 != '\0')
    {

        if(*str1 != *str2)
            return (*str1 - *str2);
        str1++;
        str2++;

    }
    return 0;
}
char* myStrcat(char* destination, const char* source) {
    // Find the end of the destination string
    char* dest_end = destination;
    while (*dest_end != '\0') {
        dest_end++;
    }

    // Copy characters from the source string to the end of the destination string
    while (*source != '\0') {
        *dest_end = *source;
        dest_end++;
        source++;
    }

    // Null-terminate the concatenated string
    *dest_end = '\0';

    return destination;
}
char * intToString(int i)
{
    char *buffer = (char *)malloc(BUFSIZ);

    sprintf(buffer, "%d", i);
    return (char *)buffer;
}
char * convertIntToMonth(int m)
{
    switch (m) {
        case 0:
            return "Jan";
        case 1:
            return "Feb";
        case 2:
            return "Mar";
        case 3:
            return "Apr";
        case 4:
            return "May";
        case 5:
            return "Jun";
        case 6:
            return "Jul";
        case 7:
            return "Aug";
        case 8:
            return "Sep";
        case 9:
            return "Oct";
        case 10:
            return "Nov";
        case 11:
            return "Dec";
        default:
            return "Invalid Month";
    }
}

