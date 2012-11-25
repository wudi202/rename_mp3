#include "stdio.h"
#include "string.h"
#include "iconv.h"
#include "util.h"

char strIDV2Encode[][15] = {
        "UTF-8",
        "UTF-16",
        "UTF-16BE",
        "ISO-8859-1",
        "ASCII",
        "gbk"   //my computer is gbk
    };


enCodeType GetUnicodeTypeByBeg(char *src, int len)
{
    enCodeType uniType = ASCII;
    if (null == src)
    {
        return uniType;
    }

    if (len <2)
    {
        return uniType;
    }

    if (0xFE == src[0])
    {
        if (0xFF == src[1])
        {
            return UTF16;
        }
        else if (0xFF == src[1])
        {
            return UTF16BE;
        }
    }

    if (len < 3)
    {
        return uniType;
    }

    if ((0xEF == src[0]) && (0xBB == src[1]) && (0xBF == src[2]))
    {
        return UTF8;
    }

    //others:
    //FE FE 00 00 for UTF-32/little endian
    //00 00 FE FF for UTF-32/big-endian
    return uniType;
}

int switchEncodeToUTF8(char *des, int outlen, char *src, int inlen, char *srcType)
{
    iconv_t cd;
    char *curdes = des;
    char *cursrc = src;
    size_t iLen = inlen;
    size_t oLen = outlen;

    if (null == des || (null == src) || (0 == inlen) || (0==outlen))
    {
        return -1;
    }

    if (null == srcType)
    {
        return -2;
    }

    cd = iconv_open("UTF-8", srcType);
    if (0 == cd )
    {
        return -3;
    }

    memset(des, 0, outlen);
    if (-1 == iconv(cd, &cursrc, &iLen, &curdes, &oLen))
    {
        return -4;
    }

    iconv_close(cd);
    return curdes-des;
}


char *getDir(char *pfile)
{
    char *strDir = null;
    if (null == pfile)
    {
        return null;
    }

    int len = strlen(pfile);
    char *cur = pfile+len-1;
    while ((*cur != '/') && (*cur != '\\') && (len>0))
    {
        cur--;
        len--;
    }

    if (len>0)
    {
        int dirLen = cur-pfile+1;
        strDir = new char[dirLen+1];
        memset(strDir, 0, dirLen+1);
        strncpy(strDir, pfile, dirLen);
    }
    return strDir;
}
