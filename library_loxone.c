//
//  library_loxone.c
//  picoc
//
//  Created by Markus Fritze on 08.03.18.
//

#include "library_loxone.h"
#include "interpreter.h"
#include <time.h>

static void Lox_debugPrintHeader(struct ParseState *Parser)
{
    printf("%4d : ", Parser->Line);
}

#define LOX_DEBUGPRINT      Lox_debugPrintHeader(Parser); printf

#pragma mark -
#pragma mark C LIBRARY FUNCTIONS

// char *getprogramname();
static void Lox_getprogramname(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getprogramname()\n");
    ReturnValue->Val->Pointer = "Programm"; // User entered name in the "Description"
}

// int lineno();
static void Lox_lineno(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("lineno()\n");
    ReturnValue->Val->Integer = Parser->Line;
}

// void exit();
static void Lox_exit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("exit()\n");
    PlatformExit(Parser->pc, 0);
}

// void errorprintf(char *format,...);
static void Lox_errorprintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("errorprintf(\"%s\")\n", Param[0]->Val->Pointer);
}

#pragma mark -
#pragma mark EXTENDED C LIBRARY FUNCTIONS

// int batoi(char *str);
static void Lox_batoi(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const char *str = Param[0]->Val->Pointer;
    LOX_DEBUGPRINT("batoi(\"%s\")\n", str);
    while(*str == ' ')
        ++str;
    ReturnValue->Val->Integer = atoi(str);
}

// float batof(char *str);
static void Lox_batof(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    char *str = Param[0]->Val->Pointer;
    LOX_DEBUGPRINT("batof(\"%s\")\n", str);
    while(*str == ' ')
        ++str;
    ReturnValue->Val->FP = atof(str);
}

// char* strstrskip(char *str,char *strfind);
static void Lox_strstrskip(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    char *str = Param[0]->Val->Pointer;
    char *strfind = Param[1]->Val->Pointer; 
    LOX_DEBUGPRINT("strstrskip(\"%s\", \"%s\")\n", str, strfind);
    str = strstr(str, strfind);
    if(str)
        str += strlen(strfind);
    ReturnValue->Val->Pointer = str;
}

// char *getxmlvalue(char *str,int index,char* name);
static void Lox_getxmlvalue(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const char *str = Param[0]->Val->Pointer;
    int index = Param[1]->Val->Integer;
    const char *name = Param[2]->Val->Pointer; 
//    LOX_DEBUGPRINT("getxmlvalue(\"%s\", %d, \"%s\")\n", str, index, name);
    LOX_DEBUGPRINT("getxmlvalue(<XML>, %d, \"%s\")\n", index, name);
    char startTag[100],endTag[100];
    sprintf(startTag, "<%s>", name);
    sprintf(endTag, "</%s>", name);
    const char *foundStart = str,*foundEnd = str;
    do {
        foundStart = strstr(foundStart, startTag);
        if(foundStart) foundStart += strlen(startTag);
        foundEnd = strstr(foundEnd, endTag);
        if(foundEnd) foundEnd += strlen(endTag);
    } while(--index >= 0);
    ReturnValue->Val->Pointer = NULL;
    if(foundEnd > foundStart) {
        foundEnd -= strlen(endTag);
        char *buf = malloc(foundEnd - foundStart + 1);
        strncpy(buf, foundStart, foundEnd - foundStart);
        buf[foundEnd - foundStart] = 0;
        ReturnValue->Val->Pointer = buf;
    }
}

// int getcpuinfo();
// Returns actual value of CPU usage in %.
static void Lox_getcpuinfo(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getcpuinfo()\n");
    ReturnValue->Val->Integer = 44;
}

// int getheapusage();
// Returns actual value of system heap in kB.
static void Lox_getheapusage(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getheapusage()\n");
    ReturnValue->Val->Integer = 10658;
}

// int getmaxheap();
// Returns maximum value of system heap in kB.
static void Lox_getmaxheap(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getmaxheap()\n");
    ReturnValue->Val->Integer = 51744;
}

// int getspsstatus();
// Returns actual number of cycles processed by SPS.
static void Lox_getspsstatus(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getspsstatus()\n");
    ReturnValue->Val->Integer = 100; // Frequency in the Loxone project settings (20-250)
}

// char *localwebservice(char *str);
// Execute webservice and return a pointer to the answer XML string.
static void Lox_localwebservice(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const char *str = Param[0]->Val->Pointer;
    LOX_DEBUGPRINT("localwebservice(\"%s\")\n", str);
    const char *responseStr = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<LL control=\"dev/sps/status\" value=\"Running 100/sec\" Code=\"200\"/>";
    char *responseBuffer = malloc(strlen(responseStr) + 1);
    strcpy(responseBuffer, responseStr);
    ReturnValue->Val->Pointer = responseBuffer;
}

#pragma mark -
#pragma mark HTTP FUNCTIONS

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

static int socket_connect(const char *host, in_port_t port, int tcp /* tcp (1) or udp (0) */)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, tcp ? IPPROTO_TCP : IPPROTO_UDP);
    if (sockfd == -1)
        return -1;
    if(tcp) {
        int on = 1;
        setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    }
    struct sockaddr_in host_addr;
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = inet_addr(host);
    if (host_addr.sin_addr.s_addr == INADDR_NONE) {
        // Server addressed by name, not by the IP?
        struct hostent *hostinfo = gethostbyname(host);
        if (hostinfo == NULL) {
            close(sockfd);
            return -1;
        }
        memcpy((char*) &host_addr.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
    }
    if (connect(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

// char *httpget(char *address,char *page);
static void Lox_httpget(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const char *server = Param[0]->Val->Pointer;
    const char *page = Param[1]->Val->Pointer;
    LOX_DEBUGPRINT("localwebservice(\"%s\", \"%s\")\n", server, page);
    int sockfd = socket_connect(server, 80, 1);
    if (sockfd == -1) {
        ReturnValue->Val->Pointer = NULL;
        return;
    }
    #define BUFFER_SIZE 1024
    size_t bufferSize = BUFFER_SIZE;
    char *buffer = malloc(bufferSize);
    sprintf(buffer, "GET %s HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "Connection: close\r\n"
                    "\r\n", page, server);  
    write(sockfd, buffer, strlen(buffer));
    ssize_t bufferCount = 0;
    while(1) {
        ssize_t bytesRead = read(sockfd, buffer + bufferCount, BUFFER_SIZE - 1);
        if(!bytesRead)
            break;
        bufferCount += bytesRead;
        buffer = realloc(buffer, bufferSize + BUFFER_SIZE);
        bzero(buffer + bufferCount, BUFFER_SIZE);
        bufferSize += BUFFER_SIZE;
    }
    shutdown(sockfd, SHUT_RDWR); 
    close(sockfd);

    char *responseBuffer = malloc(bufferCount + 1);
    strcpy(responseBuffer, buffer);
    ReturnValue->Val->Pointer = responseBuffer;
}

#pragma mark -
#pragma mark SPS FUNCTIONS

// void setlogtext(char *str);
// Writes the string contained in paramater str to the log window of Loxone Config.
static void Lox_setlogtext(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("setlogtext(\"%s\")\n", Param[0]->Val->Pointer);
}

// float getio(char *str);
// Returns the value of the speciefied virtual input or a virtual output.
static void Lox_getio(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getio(\"%s\")\n", Param[0]->Val->Pointer);
    ReturnValue->Val->FP = 0.0;
}

// int setio(char *str,float value);
// Sets the value of the speciefied virtual input or a virtual output.
static void Lox_setio(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("setio(\"%s\", %g)\n", Param[0]->Val->Pointer, Param[0]->Val->FP);
    ReturnValue->Val->Integer = 0;
}

// int getinputevent();
// Returns a bitmask which contains the changes of inputs (bit 0 = first input of object, starts with text inputs followed by analog inputs).
// A return value of 0x02 for example means that input 2 has changed.
static void Lox_getinputevent(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getinputevent()\n");
    ReturnValue->Val->Integer = 0xfff;
}

// float getinput(int input);
// Returns the value of the analog input specified in parameter input. (0 = first analog input)
static void Lox_getinput(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getinput(%d)\n", Param[0]->Val->Integer);
    ReturnValue->Val->FP = 1;
}

// char *getinputtext(int input);
// Returns the string of the text input specified in parameter input.
static void Lox_getinputtext(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getinputtext(%d)\n", Param[0]->Val->Integer);
    ReturnValue->Val->Pointer = "???";
}

// void setoutput(int output,float value);
// Sets the specified analog output to the specified value. (0 = first analog output)
static void Lox_setoutput(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("setoutput(%d, %g)\n", Param[0]->Val->Integer, Param[1]->Val->FP);
}

// void setoutputtext(int output,char *str);
// Sets the spcified text output to the string specified in parameter str. (0 = first text output)
static void Lox_setoutputtext(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("setoutputtext(%d, \"%s\")\n", Param[0]->Val->Integer, Param[1]->Val->Pointer);
}


#pragma mark -
#pragma mark TIME FUNCTIONS

// void sleep(int);
// Sleep in milliseconds.
static void Lox_sleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("sleep(%d)\n", Param[0]->Val->Integer);
    usleep(Param[0]->Val->Integer * 1000);
}

// void sleeps(int s);
// Sleep in seconds.
static void Lox_sleeps(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("sleeps(%d)\n", Param[0]->Val->Integer);
    sleep(Param[0]->Val->Integer);
}

// Number of seconds from 1.1.1970 till 1.1.2009 (start time of the Loxone server) 
static const unsigned int secondsTill1_1_2009 = 1230768000;

// unsigned int getcurrenttime();
// Get UTC time in seconds since 1.1.2009.
static void Lox_getcurrenttime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getcurrenttime()\n");
    time_t t = time(NULL);
    ReturnValue->Val->UnsignedLongInteger = t - secondsTill1_1_2009;
}

// int getyear(unsigned int time,int local);
// Get year of specified time. You can set the following parameters:
// time:
// UTC time in seconds since 2009
// local:
// Specifies time format: local time when 1, UTC when 0
static void Lox_getyear(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getyear(%u, %d)\n", Param[0]->Val->UnsignedInteger, Param[1]->Val->Integer);
    time_t t = Param[0]->Val->UnsignedInteger + secondsTill1_1_2009;
    struct tm *local;
    if(Param[1]->Val->Integer)
        local = localtime(&t);
    else
        local = gmtime(&t);
    ReturnValue->Val->UnsignedInteger = local->tm_year + 1900;
}

// int getmonth(unsigned int time,int local);
// Get month of specified time. You can set the following parameters:
// time: UTC time in seconds since 2009
// local: Specifies time format: local time when 1, UTC when 0
static void Lox_getmonth(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getmonth(%u, %d)\n", Param[0]->Val->UnsignedInteger, Param[1]->Val->Integer);
    time_t t = Param[0]->Val->UnsignedInteger + secondsTill1_1_2009;
    struct tm *local;
    if(Param[1]->Val->Integer)
        local = localtime(&t);
    else
        local = gmtime(&t);
    ReturnValue->Val->UnsignedInteger = local->tm_mon + 1;
}
 
// int getday(unsigned int time,int local);
// Get day of specified time. You can set the following parameters:
// time: UTC time in seconds since 2009
// local: Specifies time format: local time when 1, UTC when 0
static void Lox_getday(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getday(%u, %d)\n", Param[0]->Val->UnsignedInteger, Param[1]->Val->Integer);
    time_t t = Param[0]->Val->UnsignedInteger + secondsTill1_1_2009;
    struct tm *local;
    if(Param[1]->Val->Integer)
        local = localtime(&t);
    else
        local = gmtime(&t);
    ReturnValue->Val->UnsignedInteger = local->tm_mday;
}
 
// int gethour(unsigned int time,int local);
// Get hour of specified time. You can set the following parameters:
// time: UTC time in seconds since 2009
// local: Specifies time format: local when 1, UTC when 0
 static void Lox_gethour(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("gethour(%u, %d)\n", Param[0]->Val->UnsignedInteger, Param[1]->Val->Integer);
    time_t t = Param[0]->Val->UnsignedInteger + secondsTill1_1_2009;
    struct tm *local;
    if(Param[1]->Val->Integer)
        local = localtime(&t);
    else
        local = gmtime(&t);
    ReturnValue->Val->UnsignedInteger = local->tm_hour;
}

// int getminute(unsigned int time,int local);
// Get minute of specified time. You can set the following parameters:
// time: UTC time in seconds since 2009
// local: Specifies time format: local when 1, UTC when 0
 static void Lox_getminute(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getminute(%u, %d)\n", Param[0]->Val->UnsignedInteger, Param[1]->Val->Integer);
    time_t t = Param[0]->Val->UnsignedInteger + secondsTill1_1_2009;
    struct tm *local;
    if(Param[1]->Val->Integer)
        local = localtime(&t);
    else
        local = gmtime(&t);
    ReturnValue->Val->UnsignedInteger = local->tm_min;
}

// int getsecond(unsigned int time,int local)
// Get second of specified time. You can set the following parameters:
// time: UTC time in seconds since 2009
// local: Specifies time format: local when 1, UTC when 0
 static void Lox_getsecond(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("getsecond(%u, %d)\n", Param[0]->Val->UnsignedInteger, Param[1]->Val->Integer);
    time_t t = Param[0]->Val->UnsignedInteger + secondsTill1_1_2009;
    struct tm *local;
    if(Param[1]->Val->Integer)
        local = localtime(&t);
    else
        local = gmtime(&t);
    ReturnValue->Val->UnsignedInteger = local->tm_sec;
}

#include <sys/timeb.h>

static int getUTCOffset(int useLocal)
{
    if(useLocal) {
        struct timeb tp;
        ftime(&tp);
        return tp.timezone * 60;
    } else {
        return 0;
    }
}

// unsigned int gettimeval(int year,int month,int day,int hour,int minutes,int seconds,int local);
// Get UTC time in seconds since 2009. If parameters year, month, day, hour, minutes and seconds are UTC time, parameter local must be set to 0. If parameters year, month, day, hour, minutes and seconds are local time, parameter local must be set to 1.
static void Lox_gettimeval(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("gettimeval(%d, %d, %d, %d, %d, %d, %d)\n", Param[0]->Val->Integer, Param[1]->Val->Integer, Param[2]->Val->Integer, Param[3]->Val->Integer, Param[4]->Val->Integer, Param[5]->Val->Integer, Param[6]->Val->Integer);
    struct tm t;
    t.tm_year = Param[0]->Val->Integer - 1900;
    t.tm_mon = Param[1]->Val->Integer - 1;
    t.tm_mday = Param[2]->Val->Integer;
    t.tm_hour = Param[3]->Val->Integer;
    t.tm_min = Param[4]->Val->Integer;
    t.tm_sec = Param[5]->Val->Integer;
    t.tm_wday = 0;
    t.tm_yday = 0;
    t.tm_isdst = 0;
    ReturnValue->Val->UnsignedLongInteger = mktime(&t) - getUTCOffset(!Param[6]->Val->Integer) - secondsTill1_1_2009;
}

// unsigned int convertutc2local(unsigned int timeutc);
// Converts UTC time to localtime.
static void Lox_convertutc2local(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("convertutc2local(%u)\n", Param[0]->Val->UnsignedInteger);
    ReturnValue->Val->UnsignedInteger = Param[0]->Val->UnsignedInteger - getUTCOffset(1);
}
 
// unsigned int convertlocal2utc(unsigned int timelocal);
// Converts local time to UTC time.
static void Lox_convertlocal2utc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("convertlocal2utc(%u)\n", Param[0]->Val->UnsignedInteger);
    ReturnValue->Val->UnsignedInteger = Param[0]->Val->UnsignedInteger + getUTCOffset(1);
}

#pragma mark -
#pragma mark BUFFER FUNCTIONS

// int getshort(void * p,int bBigEndian);
static void Lox_getshort(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const unsigned char *pointer = Param[0]->Val->Pointer;
    const int bBigEndian = Param[1]->Val->Integer;
    LOX_DEBUGPRINT("getshort(%p, %d)\n", pointer, bBigEndian);
    if(bBigEndian)
        ReturnValue->Val->Integer = (short)((pointer[0]<<8) | pointer[1]);
    else
        ReturnValue->Val->Integer = (short)((pointer[1]<<8) | pointer[0]);
}

// int getushort(void * p,int bBigEndian);
static void Lox_getushort(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const unsigned char *pointer = Param[0]->Val->Pointer;
    const int bBigEndian = Param[1]->Val->Integer;
    LOX_DEBUGPRINT("getushort(%p, %d)\n", pointer, bBigEndian);
    if(bBigEndian)
        ReturnValue->Val->UnsignedInteger = ((pointer[0]<<8) | pointer[1]);
    else
        ReturnValue->Val->UnsignedInteger = ((pointer[1]<<8) | pointer[0]);
}

// int getint(void * p,int bBigEndian);
static void Lox_getint(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const unsigned char *pointer = Param[0]->Val->Pointer;
    const int bBigEndian = Param[1]->Val->Integer;
    LOX_DEBUGPRINT("getint(%p, %d)\n", pointer, bBigEndian);
    if(bBigEndian)
        ReturnValue->Val->Integer = ((pointer[0]<<24) | (pointer[1]<<16) | (pointer[2]<<8) | pointer[3]);
    else
        ReturnValue->Val->Integer = ((pointer[3]<<24) | (pointer[2]<<16) | (pointer[1]<<8) | pointer[0]);
}

// int getshort(void * p,int bBigEndian);
static void Lox_getuint(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const unsigned char *pointer = Param[0]->Val->Pointer;
    const int bBigEndian = Param[1]->Val->Integer;
    LOX_DEBUGPRINT("getshort(%p, %d)\n", pointer, bBigEndian);
    if(bBigEndian)
        ReturnValue->Val->UnsignedInteger = ((pointer[0]<<24) | (pointer[1]<<16) | (pointer[2]<<8) | pointer[3]);
    else
        ReturnValue->Val->UnsignedInteger = ((pointer[3]<<24) | (pointer[2]<<16) | (pointer[1]<<8) | pointer[0]);
}

// int getfloat(void * p,int bBigEndian);
static void Lox_getfloat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const unsigned char *pointer = Param[0]->Val->Pointer;
    const int bBigEndian = Param[1]->Val->Integer;
    LOX_DEBUGPRINT("getfloat(%p, %d)\n", pointer, bBigEndian);
    union {
        float a;
        int b;
    } conv;
    if(bBigEndian)
        conv.b = (pointer[0]<<24) | (pointer[1]<<16) | (pointer[2]<<8) | pointer[3];
    else
        conv.b = (pointer[3]<<24) | (pointer[2]<<16) | (pointer[1]<<8) | pointer[0];
    ReturnValue->Val->FP = conv.a;
}

// int getdouble(void * p,int bBigEndian);
static void Lox_getdouble(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const unsigned char *pointer = Param[0]->Val->Pointer;
    const int bBigEndian = Param[1]->Val->Integer;
    LOX_DEBUGPRINT("getdouble(%p, %d)\n", pointer, bBigEndian);
    union {
        double a;
        long b;
    } conv;
    if(bBigEndian)
        conv.b = ((unsigned long)pointer[0]<<56) | ((unsigned long)pointer[1]<<48) | ((unsigned long)pointer[2]<<40) | ((unsigned long)pointer[3]<<32) | ((unsigned long)pointer[4]<<24) | (pointer[5]<<16) | (pointer[6]<<8) | pointer[7];
    else
        conv.b = ((unsigned long)pointer[7]<<56) | ((unsigned long)pointer[6]<<48) | ((unsigned long)pointer[5]<<40) | ((unsigned long)pointer[4]<<32) | ((unsigned long)pointer[3]<<24) | (pointer[2]<<16) | (pointer[1]<<8) | pointer[0];
    ReturnValue->Val->FP = conv.a;
}


#pragma mark -
#pragma mark STREAM FUNCTIONS

typedef struct STREAM_STRUCT {
    int sockfd;
} STREAM_STRUCT;

// STREAM *stream_create(char* filename,int read,int append);
static void Lox_stream_create(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    const char *filename = Param[0]->Val->Pointer;
    int read = Param[1]->Val->Integer;
    int append = Param[2]->Val->Integer;
    LOX_DEBUGPRINT("stream_create(\"%s\", %d, %d)\n", filename, read, append);

    char hostName[1024];
    int port = 0;
    if(!strncmp(filename, "/dev/tcp/", 9) || !strncmp(filename, "/dev/udp/", 9)) {
        strcpy(hostName, filename+9);
        char    *endChar = hostName + strlen(hostName) - 1;
        if(*endChar == '/') *endChar-- = 0;
        endChar = strrchr(hostName, '/');
        if(endChar != NULL) {
            port = atoi(endChar+1);
            endChar[0] = 0;
        }
        printf("%s %s:%d\n", filename[5] == 't' ? "TCP" : "UDP", hostName, port);
    } else if(!strcmp(filename, "/dev/syslog")) {
        printf("NOT SUPPORTED: syslog\n");
        return;
    } else if(!strncmp(filename, "/dev/tty/", 9)) { // RS232/RS485 (only read access is possible!)
        printf("NOT SUPPORTED: RS232/RS485\n");
        return;
    } else {
        printf("NOT SUPPORTED: File access\n");
        return;
    }

    ReturnValue->Val->Pointer = NULL;
    int sockfd = socket_connect(hostName, port, port != 0);
    if (sockfd == -1)
        return;
    STREAM_STRUCT *ss = calloc(1, sizeof(STREAM_STRUCT));
    ss->sockfd = sockfd;
    ReturnValue->Val->Pointer = ss;
}

// void stream_printf(STREAM* stream,char *format, …);
static void Lox_stream_printf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("stream_printf(%p, \"%s\", ...)\n", Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

// int stream_write(STREAM* stream,void* ptr,int size);
static void Lox_stream_write(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    STREAM_STRUCT *ss = Param[0]->Val->Pointer;
    LOX_DEBUGPRINT("stream_write(%p, %p, %d)\n", ss, Param[1]->Val->Pointer, Param[2]->Val->Integer);
    printf("%s\n", Param[1]->Val->Pointer);
    ssize_t byteCount = send(ss->sockfd, Param[1]->Val->Pointer, Param[2]->Val->Integer, 0);
    if (byteCount == -1) {
        perror("send()");
    }
    ReturnValue->Val->Integer = (int)byteCount;
}

// void stream_flush(STREAM* stream)
static void Lox_stream_flush(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("stream_flush(%p)\n", Param[0]->Val->Pointer);
}

// int stream_read(STREAM* stream,void* ptr,int size,int timeout);
static void Lox_stream_read(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    STREAM_STRUCT *ss = Param[0]->Val->Pointer;
    LOX_DEBUGPRINT("stream_read(%p, %p, %d)\n", ss, Param[1]->Val->Pointer, Param[2]->Val->Integer);
    ssize_t byteCount = recv(ss->sockfd, Param[1]->Val->Pointer, Param[2]->Val->Integer, 0);
    if (byteCount == -1) {
        perror ("recv()");
    }
    printf("%s\n", Param[1]->Val->Pointer);
    ReturnValue->Val->Integer = (int)byteCount;
}

// int stream_readline(STREAM* stream,void* ptr,int maxsize,int timeout);
static void Lox_stream_readline(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("stream_readline(%p, %p, %d, %d)\n", Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer, Param[3]->Val->Integer);
}

// void stream_close(STREAM* stream);
static void Lox_stream_close(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    LOX_DEBUGPRINT("stream_close(%p)\n", Param[0]->Val->Pointer);
    STREAM_STRUCT *ss = Param[0]->Val->Pointer;
    if(ss) {
        close(ss->sockfd);
        free(ss);
    }
}


#pragma mark -

static void LoxoneSetupFunc(Picoc *pc)
{
    printf("LoxoneSetupFunc()\n");
}

// handy structure definitions
const char LoxoneDefs[] = "\
typedef struct __STREAMStruct STREAM;\
";

static struct LibraryFunction LoxoneFunctions[] =
{
    // EXTENDED C LIBRARY FUNCTIONS
    { Lox_getprogramname,   "char *getprogramname();" },
    { Lox_lineno,           "int lineno();" },
    { Lox_exit,             "void exit(void);" },
    { Lox_errorprintf,      "void errorprintf(char *format,...);" },

    { Lox_batoi,            "int batoi(char *str);" },
    { Lox_batof,            "float batof(char *str);" },
    { Lox_strstrskip,       "char* strstrskip(char *str,char *strfind);" },

    { Lox_getxmlvalue,      "char *getxmlvalue(char *str,int index,char* name);" },

    { Lox_getcpuinfo,       "int getcpuinfo();" },
    { Lox_getheapusage,     "int getheapusage();" },
    { Lox_getmaxheap,       "int getmaxheap();" },
    { Lox_getspsstatus,     "int getspsstatus();" },
    { Lox_localwebservice,  "char *localwebservice(char *str);" },

    // HTTP FUNCTIONS
    { Lox_httpget,          "char *httpget(char *address,char *page);" },

    // SPS FUNCTIONS
    { Lox_setlogtext,       "void setlogtext(char *str);" },
    { Lox_getio,            "float getio(char *str);" },
    { Lox_setio,            "int setio(char *str,float value);" },
    { Lox_getinputevent,    "int getinputevent();" },
    { Lox_getinput,         "float getinput(int input);" },
    { Lox_getinputtext,     "char *getinputtext(int input);" },
    { Lox_setoutput,        "void setoutput(int output,float value);" },
    { Lox_setoutputtext,    "void setoutputtext(int output,char *str);" },

    // TIME FUNCTIONS
    { Lox_sleep,            "void sleep(int);" },
    { Lox_sleeps,           "void sleeps(int s);" },
    { Lox_getcurrenttime,   "unsigned int getcurrenttime();" },
    { Lox_getyear,          "int getyear(unsigned int time,int local);" },
    { Lox_getmonth,         "int getmonth(unsigned int time,int local);" },
    { Lox_getday,           "int getday(unsigned int time,int local);" },
    { Lox_gethour,          "int gethour(unsigned int time,int local);" },
    { Lox_getminute,        "int getminute(unsigned int time,int local);" },
    { Lox_getsecond,        "int getsecond(unsigned int time,int local);" },
    { Lox_gettimeval,       "unsigned int gettimeval(int year,int month,int day,int hour,int minutes,int seconds,int local);" },
    { Lox_convertutc2local, "unsigned int convertutc2local(unsigned int timeutc);" },
    { Lox_convertlocal2utc, "unsigned int convertlocal2utc(unsigned int timelocal);" },

    // BUFFER FUNCTIONS
    { Lox_getshort,         "int getshort(void * p,int bBigEndian);" },
    { Lox_getushort,        "unsigned int getushort(void * p,int bBigEndian);" },
    { Lox_getint,           "int getint(void * p,int bBigEndian);" },
    { Lox_getuint,          "unsigned int getuint(void * p,int bBigEndian);" },
    { Lox_getfloat,         "float getfloat(void * p,int bBigEndian);" },
    { Lox_getdouble,        "float getdouble(void * p,int bBigEndian);" },

    // STREAM FUNCTIONS
    { Lox_stream_create,    "STREAM *stream_create(char* filename,int read,int append);" },
    { Lox_stream_printf,    "void stream_printf(STREAM* stream,char *format, ...);" },
    { Lox_stream_write,     "int stream_write(STREAM* stream,void* ptr,int size);" },
    { Lox_stream_flush,     "void stream_flush(STREAM* stream);" },
    { Lox_stream_read,      "int stream_read(STREAM* stream,void* ptr,int size,int timeout);" },
    { Lox_stream_readline,  "int stream_readline(STREAM* stream,void* ptr,int maxsize,int timeout);" },
    { Lox_stream_close,     "void stream_close(STREAM* stream);" },

    { NULL,                 NULL }
};

void LoxoneLibraryInit(Picoc *pc)
{
    IncludeRegister(pc, "loxone.h", &LoxoneSetupFunc, &LoxoneFunctions[0], LoxoneDefs);

    struct ValueType *StructFileType;

    // make a "struct __FILEStruct" which is the same size as a native FILE structure
    StructFileType = TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "__STREAMStruct"), sizeof(STREAM_STRUCT));
}
