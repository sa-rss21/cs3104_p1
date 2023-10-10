#include <fcntl.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define WRITE_SYSCALL 1
#define GETDENTS_SYSCALL 217
#define OPEN_SYSCALL 2
#define CLOSE_SYSCALL 3
#define STAT_SYSCALL 4



/*
 *
 *  SYS CALLS
 *
 */


size_t myStrlen(char *);

int myWrite(char * text)
{
    /*
     *  in-line assembly implementation of write() syscall from starter.c
     */

    long handle = 1;     // 1 for stdout, 2 for stderr, file handle from open() for files
    int ret = -1;       // Return value received from the system call
    size_t len = myStrlen(text);

    asm("syscall" : "=a" (ret) : "0"(WRITE_SYSCALL), "D"(handle), "S"(text), "d"(len) : "cc", "rcx", "r11", "memory");
    return ret;
}


int myOpen(char * path, mode_t mode)
{
    /*
     *  in-line assembly implementation of open() syscall
     */

    int ret;
    asm("syscall" 
            : "=a"(ret)
            : "0"(OPEN_SYSCALL), "D"(path), "S"(mode), "d"(S_IRUSR | S_IWUSR)
            : "cc", "rcx", "r11", "memory");
    return ret;
}


int myStat(const char * path, struct stat * statbuf)
{
    /*
     *  in-line assembly implementation of stat() syscall
     */
    int ret;
    asm("syscall" 
            : "=a" (ret) 
            : "0"(STAT_SYSCALL), "D"(path), "S"(statbuf) 
            : "cc", "rcx", "r11", "memory");

    return ret;
    
}


int myClose(int handle)
{
    /*
     *  in-line assembly implementation of close() syscall
     */

    int ret;
    asm("syscall" 
            : "=a" (ret) 
            : "0"(CLOSE_SYSCALL), "D"(handle) 
            : "cc", "rcx", "r11", "memory");
    return ret;

}


int myGetDents(int fd, char * buffer)
{
    /*
     *  in-line assembly implementation of getdents() syscall
     */

    int ret;
    asm("syscall"
            : "=a" (ret)
            : "0"(GETDENTS_SYSCALL), "D"(fd), "S"(buffer), "d"(BUFSIZ)
            : "cc", "rcx", "r11", "memory");
    return ret;
}


/*
 *
 *  STRING FUNCTIONS
 *
 */


size_t myStrlen(char * str)
{
    /*
     *  Custom implementation of strlen() 
     */

    size_t i= 0;
    while(*str != '\0')
    {
        i++;
        str++;
    }
    return i;
}


int myStrcmp(char * str1, char * str2)
{
    /*
     *  Custom implementation of strcmp() 
     */

    while(*str1 != '\0' && *str2 != '\0')
    {

        if(*str1 != *str2)
            return (*str1 - *str2);
        str1++;
        str2++;

    }
    return 0;
}


char* myStrcat(char* destination, const char* source) 
{
    /*
     *  Custom implementation of strcat() 
     */

    // Find the end of the destination string
    char* dest_end = destination;
    while (*dest_end != '\0') { dest_end++; }

    // Copy characters from the source string to the end of the destination string
    while (*source != '\0') 
    {
        *dest_end = *source;
        dest_end++;
        source++;
    }

    // Null-terminate the concatenated string
    *dest_end = '\0';

    return destination;
}


char* concatAll(char* destination, char** concat, size_t len)
{
    /*
     *  Concats a list of strings together using myStrcat() to form a larger string easier
     */

    for(int i=0; i< len; i++){ myStrcat(destination, concat[i]);}
    return destination;
}


char * intToString(int i)
{
    /*
     *  Converts an int to string
     */
    char *buffer = (char *)malloc(BUFSIZ);

    sprintf(buffer, "%d", i);
    return (char *)buffer;
}


char * convertIntToMonth(int m)
{
    /*
     *  Converts a number 0-11 to the corresponding month
     */

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


int myTolower(int c) 
{
    /*
     *  Custom implementation of tolower() 
     */

    if (c >= 'A' && c <= 'Z') {
        // Convert uppercase character to lowercase
        return c + ('a' - 'A');
    } else {
        // Character is not an uppercase letter; return as is
        return c;
    }
}


char* myStrdup(char* str) 
{
    /*
     *  Custom implementation of strdup() 
     */

    if (str == NULL)
        return NULL;

    // Allocate memory for the new string
    char* newStr = (char*)malloc(myStrlen(str) + 1);

    if (newStr == NULL) 
    {
        // Handle memory allocation failure
        return NULL;
    }

    // Copy the string into the newly allocated memory
    myStrcat(newStr, str);

    return newStr;
}


/*
 *
 *  SORTING
 *
 */

int compareIgnoreCase(char *s1, char *s2) 
{
    /*
     *  Helper function for sort algorithm
     */

    while (*s1 && *s2) {
        int diff = myTolower(*s1) - myTolower(*s2);
        if (diff != 0) {
            return diff;
        }
        s1++;

        s2++;
    }
    return myTolower(*s1) - myTolower(*s2);
}

void mySort(char *strings[], int numStrings) 
{
    /*
     *  Bubble sort algorithm for strings
     */

    int swapped;
    do 
    {
        swapped = 0;
        for (int i = 0; i < numStrings - 1; i++) 
        {
            if (compareIgnoreCase(strings[i], strings[i + 1]) > 0) 
            {
                // Swap the strings
                char *temp = strings[i];
                strings[i] = strings[i + 1];
                strings[i + 1] = temp;
                swapped = 1;
            }
        }
    }while(swapped);
}
