#include "stdio.h"
#include "memory.h"
#include "decodeinfo_mp3.h"
#include "util.h"

char IDV2Type::strIDV2Type[][5] = {
        "TEXT",
        "TENC",
        "WXXX",
        "TCOP",
        "TOPE",

        "TIT2"
    };

GetMp3Info::GetMp3Info(char *pfile)
{
    char *cur;
    pf = null;
    oldPath = null;
    err = null;
    encodeType = 0xFF;
    memset(&mID3V2Header, 0, sizeof(mID3V2Header));
    memset(&mIDV1Frame, 0, sizeof(IDV1Frame));

    if (null == pfile)
    {
        err = "the input file path is null";
        return;
    }

    int len = strlen(pfile);
    if (len < 3)
    {
        err = "the input file path is invalid";
        return;
    }

    cur = &(pfile[len-4]);
    if (strcmp(cur, ".mp3"))
    {
        err = "the file is not end with .mp3";
        return;
    }


    oldPath = new char[len];
    pf = fopen(pfile,"r");
    if (null == pf)
    {
        err == "the origin file can not be opened";
        return;
    }

    return;
}

GetMp3Info::~GetMp3Info()
{
    if (null != pf)
    {
        fclose(pf);
    }

    if (null != oldPath)
    {
        delete []oldPath;
    }
}

char* GetMp3Info::getInfofromIDV2(enIDV2Type type, int *dataLen)
{
    ID3v2Frame mID3v2Frame;
    int count;
    int iDataLen;
    int dataRead = 0;

    if (null != err)
    {
        return null;
    }
    const char *strType = IDV2Type::getStrType(type);
    //const char *strType = IDV2Type.getStrType(type)；

    if (0 == mID3V2Header.Identify[0])
    {
        fseek(pf, 0, SEEK_SET); //设置从头再来
        count = fread(&mID3V2Header, sizeof(char), sizeof(ID3v2Header), pf);
        if (count != sizeof(ID3v2Header))
        {
            err = "read ID3V2 head error";
            return null;
        }
        ID3V2_frame_size = (int)(mID3V2Header.Size[0] & 0x7F) << 21
                 | (int)(mID3V2Header.Size[1] & 0x7F) << 14
                  | (int)(mID3V2Header.Size[2] & 0x7F) << 7
                  | (int)(mID3V2Header.Size[3] & 0x7F) + 10;
    }

    if ((strncmp(mID3V2Header.Identify, "ID3", 3))
        || (mID3V2Header.Ver != 3))
    {
        return null;
    }

    (void)fseek(pf, sizeof(ID3v2Header), SEEK_SET);

    while (dataRead < ID3V2_frame_size)
    {
        count = fread(&mID3v2Frame, sizeof(char), sizeof(ID3v2Frame), pf);
        dataRead += count;
        if (count != sizeof(ID3v2Frame))
        {
            err = "read ID3V2Frame head error";
            return null;
        }

        //如果第一个字节已经变成了0xFF，说明已经开始处理实际的数据帧了
        if ((0xFF == mID3v2Frame.FrameID[0]) || (0 == mID3v2Frame.FrameID[0]))
        {
            return null;
        }

        //data length
        iDataLen = mID3v2Frame.Size[0]*0x100000000 + mID3v2Frame.Size[1]*0x10000
                + mID3v2Frame.Size[2]*0x100 + mID3v2Frame.Size[3];
        //find
        if (!strncmp(mID3v2Frame.FrameID, strType, 4))
        {
            char *strData = new char[iDataLen+1];
            char *temp;
            strData[iDataLen] = '\0';
            count = fread(strData, sizeof(char), iDataLen, pf);
            *dataLen = iDataLen;
            return strData;
        }
        fseek(pf, iDataLen, SEEK_CUR);
        dataRead += iDataLen;
    }
    return null;
}

char* GetMp3Info::getInfofromIDV1(enIDV1Type type)
{
    char *strData = null;
    if (null != err)
    {
        return null;
    }

    if (strncmp(mIDV1Frame.Header, "TAG", 3))
    {
        fseek(pf, -1L*sizeof(IDV1Frame), SEEK_END);

        fread(&mIDV1Frame, sizeof(char), sizeof(IDV1Frame), pf);
    }

    if (strncmp(mIDV1Frame.Header, "TAG", 3))
    {
        return null;
    }

    char *temp = null;
    int len = 0;
    switch (type)
    {
       case ARTIST:
       {
           temp = mIDV1Frame.Artist;
           len = 30;
           break;
       }
       case TITLE:
       {
           temp = mIDV1Frame.Title;
           len = 30;
           break;
       }
       case YEAR:
       {
           temp = mIDV1Frame.Year;
           len = 4;
           break;
       }
       default:
           break;
    }

    if (null != temp)
    {
        strData = new char[len];
        memcpy(strData, temp, len);
    }
    return strData;
}

char* GetMp3Info::getTitle(int *pDataLen, enCodeType *encode)
{
    int len = 0;
    char *newStrTitle = null;
    char *strTitle = getInfofromIDV2(TIT2, &len);
    if (null == strTitle)
    {
        newStrTitle = getInfofromIDV1(TITLE);
        *pDataLen = 30;
        *encode = GetUnicodeTypeByBeg(newStrTitle, 30);
    }
    else if (len <=1)
    {
        delete []strTitle;
        return null;
    }
    else
    {
        int hasModified = 0;
        newStrTitle = new char[len];
        memset(newStrTitle, 0, len);
        switch (strTitle[0])
        {
        case 0:    //0000代表字符使用ISO-8859-1编码方式；
               *encode = ISO_8859_1;
               break;
        case 1:    //0001代表字符使用UTF-16编码方式；
               *encode = UTF16;
               //give some modification
               if ((0xFF == (unsigned char)strTitle[1]) && (0xFE != (unsigned char)strTitle[2]))
               {
                   //if IDV2 is not good, we use IDV1 first
                   if (null != (newStrTitle = getInfofromIDV1(TITLE)))
                   {
                        *pDataLen = 30;
                        *encode = GetUnicodeTypeByBeg(newStrTitle, 30);
                        hasModified = 1;
                   }
                   else //below is the correct of IDV2
                   {

                       int index = 3;
                       while ((0xFE != (unsigned char)strTitle[index]) && (index < len-1))
                       {
                           index ++;
                       }

                       //means we finally find the 0xFE
                       if (index != len-1)
                       {
                           hasModified = 1;
                           newStrTitle[0] = 0xFF;
                           memcpy(newStrTitle+1, strTitle+index, len-index-1);
                           *pDataLen = len-index+(len-index)%2;  //for UTF-16, this must be the mulitply of 2
                       }
                   }
               }
               break;
        case 2:    //0002代表字符使用 UTF-16BE编码方式；
            *encode = UTF16BE;
               break;
        case 3:    //0003代表字符使用UTF-8编码方式。
               *encode = UTF8;
               break;
        default:
              *encode = ASCII;
               break;
        }
        if (0 == hasModified)
        {
            memcpy(newStrTitle, strTitle+1, len-1);
            *pDataLen = len-1;
        }
        delete []strTitle;
    }
    return newStrTitle;
}

enCodeType GetMp3Info::GetUnicodeTypeByBeg(char *src, int len)
{
    enCodeType uniType = GBK;
    if (null == src)
    {
        return uniType;
    }

    if (len <2)
    {
        return uniType;
    }

    if ((0xFF == src[0]) && (0xFE == src[1]))
    {
        return UTF16;
    }

    if ((0xFE == src[0]) && (0xFF == src[1]))
    {
        return UTF16BE;
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

char* GetMp3Info::getError()
{
    return err;
}


