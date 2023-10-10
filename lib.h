
/* library functions written bt Rex Stayer-Suprick*/

int myPrint(char *);
int myStrcmp(char* str1, char * str2);
int myOpen(char* , mode_t);
int myClose(int);
int myGetDents(int, char *);
int myStat(const char *, struct stat *);
char* myStrcat(char* destination, const char* source);
char * convertIntToMonth( int m);
char* concatAll(char*, char**, size_t);

