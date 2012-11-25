#ifndef DECODEINFO_MP3
#define DECODEINFO_MP3

#include "stdio.h"
#include "util.h"

struct ID3v2Header //标签头，10个字节，通过解析这段信息我们可以知道一个MP3文件是不是有ID3v2信息，如果有我们就知道了ID3v2的数据体的总长度。
{
    char Identify[3];       // ID3v2固定标志：ID3
    char Ver;               // 主版本号，ID3v2就是3
    char Rever;             // 副版本号，一般都为0
    char Flag;              // 标志位，一般为0，字义为abc00000
                            //  a-表示是否使用Unsynchronisation
                            //  b-表示是否有扩展头部，一般没有，所以一般不设置
                            //  c-表示是否为测试标签（99.9%的标签都不是测试用的，所以一般不设置）

    unsigned char Size[4];           // 标签大小，一共四个字节，但每个字节只使用7位，最高位不使用恒为0，所以格式如下：
                            // 0xxxxxxx 0xxxxxxx 0xxxxxxx 0xxxxxxx，计算大小时要将0去掉，得到一个28位的二进制数，就是标签的大小.
                            // ID3size =(Size[0]&0x7F)*0x200000 +(Size[1]&0x7F)*0x400 +(Size[2]&0x7F)*0x80 +(Size[3]&0x7F);
};

/* FrameID标志内容
 *
 * TEXT： 歌词作者       TENC： 编码            WXXX： URL链接(URL)            TCOP： 版权(Copyright)     TOPE： 原艺术家
 * TCOM： 作曲家        TDAT： 日期            TPE3： 指挥者               TPE2： 乐队                    TPE1： 艺术家相当于ID3v1的Artist
 * TPE4： 翻译（记录员、修改员）            TYER： 即ID3v1的Year       USLT： 歌词                    TSIZ： 大小
 * TALB： 专辑相当于ID3v1的Album           TIT1： 内容组描述         TIT2： 标题相当于ID3v1的Title
 * TIT3： 副标题        TCON： 流派（风格）相当于ID3v1的Genre          AENC： 音频加密技术            TSSE： 编码使用的软件（硬件设置）
 * TBPM： 每分钟节拍数 COMM： 注释相当于ID3v1的Comment                    TDLY： 播放列表返录            TRCK： 音轨（曲号）相当于ID3v1的Track
 * TFLT： 文件类型       TIME： 时间　       TKEY： 最初关键字         TLAN： 语言                    TLEN： 长度
 * TMED： 媒体类型       TOAL： 原唱片集      TOFN： 原文件名              TOLY： 原歌词作者         TORY： 最初发行年份
 * TOWM： 文件所有者（许可证者）            TPOS： 作品集部分         TPUB： 发行人               TRDA： 录制日期
 * TRSN： Intenet电台名称                    TRSO： Intenet电台所有者  UFID： 唯一的文件标识符 　    TSRC： ISRC（国际的标准记录代码）                　 　
 */
struct ID3v2Frame //标签帧，10个字节
{
    char FrameID[4];        // 标志对照符，如TEXT，TOPE，TDAT....
    unsigned char Size[4];           // 帧体的大小，按照正常的8位存储的，FSize = Size[0]*0x100000000 + Size[1]*0x10000 + Size[2]*0x100 + Size[3];

    char Flag[2];           // 存放标志，另外10位为0　但大部分的情况下16位都为0就可以了，格式如下：
                            //  a-标签保护标志，设置时认为此帧作废
                            //  b-文件保护标志，设置时认为此帧作废
                            //  c-只读标志，设置时认为此帧不能修改（目前好像没有看到过）
                            //  i-压缩标志，设置时一个字节存放两个BCD码表示数字
                            //  j-加密标志（好像不太实用）
                            //  k-组标志，设置时说明此帧和其它的某帧是一组。
};

struct IDV1Frame
{
    char Header[3];    /* 标签头必须是"TAG"否则认为没有标签 */
    char Title[30];    /* 标题 */
    char Artist[30];   /* 作者 */
    char Album[30];    /* 专集 */
    char Year[4];      /* 出品年代 */
    char Comment[28];  /* 备注 */
    char reserve;      /* 保留 */
    char track;;
    char Genre;        /* 类型 */
};

enum enIDV1Type
{
    TITLE,   /* 标题 */
    ARTIST,  /* 作者 */
    ALBUM,   /* 专集 */
    YEAR,    /* 出品年代 */
    COMMENT, /* 备注 */
    TRACK,   /* 音轨 */
    GENRE    /* 类型 */
};

enum enIDV2Type
{
    TEXT,
    TENC,
    WXXX,
    TCOP,
    TOPE,

    TIT2
};

class IDV2Type
{
public:
    static char strIDV2Type[][5];
    //static char strIDV2Encode[][15];
public:
    IDV2Type(){};
    ~IDV2Type(){};
    static const char *getStrType(enIDV2Type type)
    {
        return strIDV2Type[type];
    };
};

class GetMp3Info
{
    FILE *pf;
    char *oldPath;
    char *err;
    ID3v2Header mID3V2Header;
    IDV1Frame mIDV1Frame;
    int ID3V2_frame_size;
    int encodeType;

public:
    GetMp3Info(char *pfile);
    ~GetMp3Info();
    //如下外部获取的信息内存都需要在外部释放
    //读取对应的标题信息
    char* getTitle(int *pDataLen, enCodeType *encode);

    //读取对应的演唱者信息
    char* getTOPE(){return 0;}

    char *getError();

    static enCodeType GetUnicodeTypeByBeg(char *src, int len);

private:
    char* getInfofromIDV2(enIDV2Type type, int *dataLen);

    char* getInfofromIDV1(enIDV1Type type);

};

#endif
