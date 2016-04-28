
/*****************************************************************
 * Copyright (C) 2010 Maipu Communication Technology Co.,Ltd.*
 ******************************************************************
 * serialized.c
 *
 * DESCRIPTION:
 *         数据包格式的序列化和反序列化模块
 * AUTHOR:
 *         mr_zhao,mr_gong,mr_hu
 * CREATED DATE:
 *         2016.4.28
 * REVISION:
 *         1.0
 * MODIFICATION HISTORY
 * --------------------
 * $Log:$
 * *****************************************************************/


#include "common.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#define PKTCONTENTLINE 2

/*****************************************************************
 * DESCRIPTION:
 *       对报文的解析函数模块
 * INPUTS:
 *       pktdata-服务器端接收的UPD待解析的数据包
 *       pUserInfo-从用户请求数据包中获取账号密码；
 * OUTPUTS:
 *       pUserInfo-返回用户的账号密码
 * RETURNS:
 *       无返回值，直接对传入指针进行处理
 *****************************************************************/

void deserialized
(
 char *pktdata,
 userInfo *pUserInfo
 )
{
    INT16 * len = (INT16 *)(pktdata + TOTALLENGTHOFFSET);
    (*len) = ntohs(* (INT16 *)len); /*字节序返回*/
    
    INT8 *pos  = pktdata; /* UDP数据包的流指针*/
    INT16 pktlength = *(INT16 *)(pos + TOTALLENGTHOFFSET); /*获取报文总长度*/
    INT8 *pktTail = pos + pktlength; /*pktTail用来作指针pos的越界检查*/
    
    pos = pos + sizeof(struct pktHeader); /*指针pos指向pkt的内容*/
    
    while (pos != pktTail)
    {
        INT8 type = (*pos);
        INT8 contentLength = * (INT8 *)(pos + CONTENTTYPEOFFSET); /*解析报文正文的type8*/
        
        /* 根据type类型解析报文内容，输出到userInfo中*/
        switch (type) {
            case USERNAME:
                memcpy(pUserInfo->name, pos + CONTENTHEADLEN, contentLength - CONTENTHEADLEN);
                pos += contentLength;
                break;
            case PASSWORD:
                memcpy(pUserInfo->password, pos + CONTENTHEADLEN, contentLength - CONTENTHEADLEN);
                pos += contentLength;
                break;
            case PREVILAGE:
                pUserInfo-> previlage = * (INT8 *)(pos + CONTENTHEADLEN);
                pos = pos + contentLength;
                break;
            default:
                printf("cannot get the correct content from the pakage");
                break;
        }
        
    }
    return;
}

/*****************************************************************
 * DESCRIPTION:
 *        客户端序列化函数
 * INPUTS:
 *        pktdata-UPD网络数据包（大端）
 *        pcontent-UPD数据包结构体数据
 * OUTPUTS:
 *        pktdata-网络流后的序列化数据
 * RETURNS:
 *        LEN-客户端序列化后的数据包长度
 *****************************************************************/

INT16 clientSerialized(char *pktdata,pktcontent *pcontent)
{
    
    pktHeader header;
    INT8 * pos = pktdata;
    
    header.code = PKT_REQUEST;
    header.id = ID;
    memset(header.md5Auth, 0, AUTHLENTH);
    
    INT16 TotalVLength = 0;

    for( int i = 0; i < PKTCONTENTLINE ; i++){
        TotalVLength += pcontent[i].lengthItem;
    }
    
    header.lengthPkt = TotalVLength + HEADOFFSET;
    
    header.lengthPkt = htons(header.lengthPkt);
    
    memcpy(pktdata, &header, HEADOFFSET);
    pos += HEADOFFSET;
    for( int i = 0; i < PKTCONTENTLINE ; i++){
        memcpy(pos,pcontent+i,pcontent[i].lengthItem);
        pos += pcontent[i].lengthItem;
    }
    
    return TotalVLength + HEADOFFSET;
    
}

/*****************************************************************
 * DESCRIPTION:
 *        服务器端序列化函数
 * INPUTS:
 *        pktdata-UPD网络数据包（大端）
 *        pcontent-UPD数据包结构体数据
 *        code-数据包类型码，1-request，2-access，3-reject
 * OUTPUTS:
 *        pktdata-本地数据结构网络序列化后的流数据
 *        LEN-序列化后的数据包长度
 * RETURNS:
 *        LEN-序列化后的数据包成都
 *
 *****************************************************************/

INT16 serverSerialized(char *pktdata,pktcontent *pcontent,int code){
    pktHeader header; /*报文头部*/
    INT8 *pos = pktdata; /*报文流指针*/
    INT8 type  = pcontent->type;
    header.code = code;
    header.id = ID;
    memset(header.md5Auth, 0, AUTHLENTH);
    
    /*对报文正文内容根据服务器接收到的TYPE值进行序列化处理 */
    
    switch (type) {
        case PASSWORD:
            header.lengthPkt = pcontent->lengthItem + HEADOFFSET;
            header.lengthPkt = htons(header.lengthPkt);
            memcpy(pktdata, &header, HEADOFFSET);
            pos += HEADOFFSET;
            memcpy(pos, pcontent, pcontent->lengthItem);
            
            break;
        case PREVILAGE:
            header.lengthPkt = pcontent->lengthItem + HEADOFFSET;
            header.lengthPkt = htons(header.lengthPkt); /*大小端转换*/
            memcpy(pktdata, &header, HEADOFFSET);
            pos += HEADOFFSET;
            memcpy(pos, pcontent, pcontent->lengthItem);
            break;
            
        default:
            printf("erro header type");
            break;
    }
    
    
    return HEADOFFSET + pcontent->lengthItem;
    
}

