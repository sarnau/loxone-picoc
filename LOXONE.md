#  Loxone specific functions

The #define LOXONE is available thoughout the code.

C LIBRARY FUNCTIONS
int strfind(char *str,char *strfind,int pos);
 
FILE FUNCTIONS
char *tmpnam(char* str);
 
HTTP FUNCTIONS
char*httpget(char*address,char*page);

BUFFER FUNCTIONS
int getshort(void * p,int bBigEndian);
unsigned int getushort(void * p,int bBigEndian);
int getint(void * p,int bBigEndian);
unsigned int getuint(void * p,int bBigEndian);
float getfloat(void * p,int bBigEndian);
float getdouble(void * p,int bBigEndian);
