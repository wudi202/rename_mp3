#ifndef SCANDIR
#define SCANDIR

#include "stdio.h"
#include "string.h"
#include "dirent.h"
#include "sys/stat.h"
#include "util.h"

class ScanDir
{
private:
    char *path;
    char *curpath;
    FILE *curDir;
    DIR  *dir;
    int   isScanend;
    char tempPath[100];
    char *temp;

public:
    ScanDir(){path=null; curpath=null; curDir=null; dir=null; isScanend=0;memset(tempPath, 0, 100);}
    ScanDir(char *path2){path = path2; curpath=null; curDir=null; dir=null; isScanend=0;memset(tempPath, 0, 100);}
    ~ScanDir(){}

public:
    char* getNextSubFile();
};

#endif
