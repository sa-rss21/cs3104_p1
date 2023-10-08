#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <stdio.h>


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

void myWrite(char * text)
{
    long handle = 1;     // 1 for stdout, 2 for stderr, file handle from open() for files
    long ret = -1;       // Return value received from the system call
    size_t len = myStrlen(text);

    //uses inline with shortcuts to write to screen
    asm("syscall" : "=a" (ret) : "0"(WRITE_SYSCALL), "D"(handle), "S"(text), "d"(len) : "cc", "rcx", "r11", "memory");
}
int myOpen(char * path, mode_t mode)
{
    return syscall(OPEN_SYSCALL, path, mode, S_IRUSR | S_IWUSR);
}
int myStat(const char * path, struct stat * statbuf)
{
    if (syscall(STAT_SYSCALL, path, statbuf) == 0) {
        return 0; // Success
    } else {
        return -1; // Error
    }
    
}
int myClose(int fd)
{
    return syscall(CLOSE_SYSCALL, fd);
}
int myGetDents(int fd, char * buffer)
{
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

