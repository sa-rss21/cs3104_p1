
/* library functions written bt Rex Stayer-Suprick*/

int myPrint(char *);
int myWrite(char * text);
int myStrcmp(char* str1, char * str2);
int myOpen(char* , mode_t);
int myClose(int);
int myGetDents(int, char *);
int myStat(const char *, struct stat *);
char* myStrcat(char* destination, const char* source);
char * convertIntToMonth( int m);
char* intToString(int num);
char* concatAll(char*, char**, size_t);
int compare_case_insensitive(const void *a, const void *b);
int compareIgnoreCase(const char *str1, const char *str2);
void mySort(char *strings[], int numStrings);
char* myStrdup(const char* str);

