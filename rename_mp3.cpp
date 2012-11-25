#include "stdio.h"
#include "decodeinfo_mp3.h"
#include "string.h"
#include "scan_dir.h"
#include "iconv.h"
#include "util.h"

extern char strIDV2Encode[][15];

int main(int argc, char* argv[])
{
    char *strTitle;
    enCodeType mencodeType;
    char strDecodeTitle[200]={0};

    for (int i=1; i<argc; i++)
    {
        char *path;
        int titleLen = 0;
        printf("The input dir is %s\n", argv[i]);
        ScanDir dir_scan(argv[i]);
        while(path = dir_scan.getNextSubFile())
        {
            printf("    The current file is %s\n", path);
            GetMp3Info *mMp3 = new GetMp3Info(path);
            strTitle = mMp3->getTitle(&titleLen, &mencodeType);

            if (null == strTitle)
            {
                continue;
            }
            else if (strlen(strTitle) > 150)
            {
                delete[] strTitle;
                continue;
            }
            memset(strDecodeTitle, 0, 200);
            //current we just handle the locale is utf-8
            if ((UTF8 != mencodeType) && (ASCII != mencodeType))
            {
                int desLen = 200;
                int res = switchEncodeToUTF8(strDecodeTitle, desLen, strTitle, titleLen, strIDV2Encode[mencodeType]);
                delete []strTitle;
                if (res < 0)
                {
                    //something error!
                    continue;
                }
                //we can not use the res as the length directly, because when we use the ID3V1, the len will be bigger
                //we can only the strlen to get the len again
                //titleLen = res;
                strTitle = strDecodeTitle;
            }
            else
            {
                memcpy(strDecodeTitle, strTitle, strlen(strTitle));
                delete[] strTitle;
                strTitle = strDecodeTitle;
            }
            //for utf-8 or ascii,we can use strlen to get the string length
            titleLen = strlen(strTitle);
            //printf("1:this path is %s\n,and the lenth is %d", path, strlen(path));
            //printf("2:this title is %s\n", strTitle);


            {
                char *strDir = getDir(path);
                //printf("%s", strDecodeTitle);
                char *strNewname;
                if (null != strDir)
                {
                    //printf("3:this dir is %s\n", strDir);
                    strNewname = new char[titleLen+strlen(strDir)+5];
                    memset(strNewname,0,titleLen+strlen(strDir)+5);
                    char *curIndex = strNewname;
                    memcpy(curIndex, strDir, strlen(strDir));
                    curIndex += strlen(strDir);
                    //printf("4:the added path1 is %s\n, and the strlen is %d\n", strNewname, strlen(strDir));
                    memcpy(curIndex, strTitle, titleLen);
                    curIndex += titleLen;
                    //printf("5:the added path1 is %s\n", strNewname);
                    memcpy(curIndex, ".mp3", 4);
                    int result = rename(path, strNewname);
                    //printf("%d", result);
                    delete []strDir;
                    strDir = null;

                }
                else
                {
                    strNewname = new char[titleLen+5];
                    char *curIndex = strNewname;
                    strNewname[titleLen+4] = '\0';
                    memcpy(curIndex, strTitle, titleLen);
                    curIndex += titleLen;
                    memcpy(curIndex, ".mp3", 4);
                    int result = rename(path, strNewname);
                    //printf("%d", result);
                }
                printf("    The new file name is %s\n", strNewname);
                delete []strNewname;
            }

            delete mMp3;
        }
    }
    return 1;
}
