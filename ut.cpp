#include "stdio.h"
#include "decodeinfo_mp3.h"
#include "string.h"
#include "scan_dir.h"
#include "iconv.h"
#include "util.h"


extern char *getDir(char *pfile);
extern int switchEncodeToUTF8(char *des, int outlen, char *src, int inlen, char *srcType);

//for test the function
//argv[i] means test the case of index i
//int main(int argc, char *argv[])
int main2()
{
    int whichTest = 0;
    #if 0
    if (argc < 2)
    {
        printf("the input args is too less!");
    }

    if (1 == *argv[1])
    #endif
    //dir_scan.getNextSubFile
    if (1 == whichTest)
    {
            char *path = null;
            ScanDir dir_scan("/home/ftp/ftp/t1");
            while(path = dir_scan.getNextSubFile())
            {
                printf("path=%s\n", path);
            }
    }

    //getDir
    if (2 == whichTest)
    {
        char *dir = null;
        #if 0
        dir = getDir("/home/ftp/ftp/t1/1.mp3");
        if (null!=dir)
        {
            printf("1:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }

        dir = getDir("/home/ftp/ftp/t1");
        if (null!=dir)
        {
            printf("2:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }

        dir = getDir("/home/ftp/ftp/t1");
        if (null!=dir)
        {
            printf("3:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }

        dir = getDir("/home/ftp/ftp/t1/");
        if (null!=dir)
        {
            printf("4:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }

        dir=getDir("/home/ftp/ftp/t1/你好");
        if (null!=dir)
        {
            printf("5:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }

        dir = getDir("/home/ftp/ftp/t1/你好/");
        if (null!=dir)
        {
            printf("6:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }

        dir = getDir("/home/ftp/ftp/t1/你好/1.mp3");
        if (null!=dir)
        {
            printf("7:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }
        #endif
        char *path = "你好/我的歌声里(DEMO).mp3";
        dir = getDir(path);
        if (null!=dir)
        {
            printf("8:dir=%s\n", dir);
            printf("strlen=%d\n",strlen(dir));
            delete []dir;
        }
    }

    //mMp3.getTitle   IDV2
    if (3 == whichTest)
    {
        char *path="/home/ftp/ftp/t1/你好/1.mp3";
        int titleLen = 0;
        enCodeType mencodeType=UTF8;
        GetMp3Info mMp3(path);
        char *strTitle = mMp3.getTitle(&titleLen, &mencodeType);

        if (null == strTitle)
        {
            printf("strTitle=null:1");
            return -1;
        }
        if ((UTF8 != mencodeType) && (ASCII != mencodeType))
        {
                char strDecodeTitle[200]={0};
                int desLen = 200;
                int res = switchEncodeToUTF8(strDecodeTitle, desLen, strTitle, titleLen, strIDV2Encode[mencodeType]);
                if (null == strTitle)
                {
                    printf("strTitle=null:2");
                    return -1;
                }
                int realLen = strlen(strDecodeTitle);
                printf("we get the decoded title:%s", strDecodeTitle);
        }
        else
        {
                printf("we get the title:%s", strTitle);

        }
        delete []strTitle;
    }

    //getDir
    if (4== whichTest)
    {
        ScanDir dir_scan("/home/ftp/ftp/t1/你好");
        char *path;
        while(path = dir_scan.getNextSubFile())
        {
            printf("path is %s", path);
            char *strDir = getDir(path);
            printf("dir is %s, and the length is %d", strDir, strlen(strDir));
        }
    }

    //convert
    if (0 == whichTest)
    {
        char strDecodeTitle[200]={0};
        int desLen=200;
        char strTitle[200] = {0xFF, 0xFE,0x60,0x4f,0xc6,0x51,0x07,0x59,0x7d,0x59,0x86,0x4e};
        int titleLen=15;
        int res = switchEncodeToUTF8(strDecodeTitle, desLen, strTitle, titleLen, "UTF-16");

        printf("res=%d\n",res);
        printf("the decodetitle is %s\n", strDecodeTitle);
    }
    return 1;
}
