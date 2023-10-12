
/* library functions written bt Rex Stayer-Suprick*/

int myPrint(char *);
int myWrite(char * text);
int myWriteFile(int, char *, size_t);
int myStrcmp(char* str1, char * str2);
int myOpen(char* , mode_t);
int myClose(int);
int myRead(int, char *, size_t);
int myGetDents(int, char *);
int myStat(const char *, struct stat *);
char* myStrcat(char* destination, const char* source);
char * convertIntToMonth( int m);
char* intToString(int num, int leading);
char* concatAll(char*, char**, size_t);
int compare_case_insensitive(const void *a, const void *b);
int compareIgnoreCase(const char *str1, const char *str2);
void mySort(char *strings[], int numStrings);
char* myStrdup(const char* str);
int myStrlen(char *);
void padString(char *dest, char *src, int width);
void formatOutputls(struct stat st, char *name);
int touch(char *);
int myUnlink(char*);
