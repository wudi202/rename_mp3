#include "scan_dir.h"
#include "stdio.h"
#include "string.h"
#include "sys/stat.h"

#if 0
getfather(char *path)
{
    int i = 0;
    if (null == path)
    {
        return null;
    }

    int len = strlen(path);

    for (i=len-1; i>=0; i--)
    {
        if (path[i]=='/')
        {
            break;
        }
    }

    if (i >=0)
    {
        char *temp = new char[i+2];
        strncpy()
    }
}

char* ScanDir::getNextSubFile()
{
    struct stat s;
    struct dirent *dt;
    char tempPath[100] = {0};
    char *temp;

    if (null == path)
    {
        return null;
    }

    if (1 == isScanend)
    {
        isScanend = 0;
        curpath = null;
        return null;
    }

    if (null == dir)
    {
        if (null != curpath)
        {
            curpath = path;
        }

        if(lstat(curpath, &s) < 0)
        {
            printf("lstat   error\n");
        }
        if (!S_ISDIR(s.st_mode))
        {
            isScanend = 1;
            return path;
        }
        else
        {
            dir = opendir(curpath);
            if (null == dir)
            {
                return null;
            }
        }
    }

    while(dt = readdir(dir))
    {
        int needdel = 0;
        if (dt->d_name[0]='.')
        {
            continue;
        }

        if (strlen(curpath)+strlen(dt->d_name)+2 > 100)
        {
            temp = new char[strlen(curpath)+strlen(dt->d_name)+2]
            needdel = 1;
        }
        else
        {
            temp = tempPath;
        }
        strcpy(temp, curpath);
        strcat(temp, '/');
        strcpy(temp, dt->d_name);

        if(lstat(temp, &s) < 0)
        {
            printf("lstat error\n");
        }
        if (!S_ISDIR(s.st_mode))
        {
            return temp;
        }

        curpath = temp;
        closedir(dir);
        dir = opendir(curpath);
    }

    if (curpath ==path)
    {
        isScanend = 1;
        return null;
    }

    char *oldpath = curpath;
    curpath = getfather(curpath);
    dir = opendir(curpath);

    while(dt = readdir(dir))
    {
        if (dt->d_name[0]='.')
        {
            continue;
        }
        if (strcmp(dt->d_name, oldpath))
        {
            continue;
        }
        break;
    }

    if (null == dt)
    {
    }







    closedir(dir);
    dir = opendir(curpath);




    if (!S_ISDIR(s.st_mode))
    {
        return path;
    }

    if (null == dir)
                char strDecodeTitle[200]={0};
                int desLen = 200;
                int res = switchEncodeToUTF8(strDecodeTitle, desLen, strTitle, titleLen, IDV2Type::strIDV2Encode[mencodeType]);
                if (null == strTitle)
                {
    {
        dir = opendir(path);
    }

    while(dt = readdir(dir))
    {
        if (dt->d_name[0]='.')
        {
            continue;
        }
        chdir(path)
    }

}
#endif

//只遍历读取当前一级目录下的情况，多级目录目前不考虑，
//当读取到null表示结束，后面继续读取的话又是从头开始
char* ScanDir::getNextSubFile()
{
    struct stat s;
    struct dirent *dt;

    if (null == path)
    {
        return null;
    }

    if (1 == isScanend)
    {
        isScanend = 0;
        return null;
    }

    if (null == dir)
    {
        if(lstat(path, &s) < 0)
        {
            printf("lstat   error\n");
        }
        if (!S_ISDIR(s.st_mode))
        {
            isScanend = 1;
            return path;
        }
        else
        {
            dir = opendir(path);
            if (null == dir)
            {
                return null;
            }
        }
    }

    while(dt = readdir(dir))
    {
        if (dt->d_name[0]=='.')
        {
            continue;
        }

        if (strlen(path)+strlen(dt->d_name)+2 > 100)
        {
            continue;
        }
        else
        {
            temp = tempPath;
        }

        //printf("getpath:1=%s\n", path);
        char *cur = temp;
        strcpy(cur, path);
        cur += strlen(path);
        strcat(cur, "/");
        cur += 1;
        //printf("getpath:2=%s\n", temp);
        strcpy(cur, dt->d_name);
        //printf("getpath:3=%s\n", temp);

        if(lstat(temp, &s) < 0)
        {
            printf("lstat error\n");
        }
        if (!S_ISDIR(s.st_mode))
        {
            return temp;
        }
    }
    closedir(dir);
    return null;
}

