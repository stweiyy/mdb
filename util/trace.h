#ifndef _TRACE_H_
#define _TRACE_H_

#include <string>
#ifdef __cplusplus
extern "C" {
#endif


typedef enum tracelevel{
    INFO=1,
    WARNING,
    ERROR
}tracelevel;

#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define TRACEBUF 1024

#ifdef __cplusplus
}
#endif

#include <map>
#include "../mdbtype.h"

class Tracer{
public:
    Tracer(std::string tf):tracefile(tf){};
    Tracer(){
	tracefile="";
    }
    void TraceInfo(tracelevel level,const char *fmt,...);
	static void tracePrint(tracelevel level,const char *fmt,...);
	static void printTbls(std::map<std::string,Table *> tbls);
private:
    std::string tracefile;
};


#endif //_TRACE_H
