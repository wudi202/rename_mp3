#ifndef DECODEMP3_UTIL
#define  DECODEMP3_UTIL

#include "stdio.h"


#ifndef null
#define null 0
#endif


enum enCodeType
{
    UTF8,
    UTF16,
    UTF16BE,
    ISO_8859_1,
    ASCII,
    GBK   //this means the type is the same with the operator system
};

enCodeType GetUnicodeTypeByBeg(char *src, int len);
int switchEncodeToUTF8(char *des, int outlen, char *src, int inlen, char *srcType);
char *getDir(char *pfile);

extern char strIDV2Encode[][15];
#endif

