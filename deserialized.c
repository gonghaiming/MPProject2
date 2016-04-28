#include <stdio.h>
#include "common.h"
#include <string.h>
#include <arpa/inet.h>

#define HEADOFFSET 20 /*UDP数据包正文偏移*/
#define TOTALLENGTHOFFSET 2 /*包长度偏移地址*/
#define CONTENTHEADLEN 2
#define CONTENTTYPEOFFSET 1

#define USERNAME '1'
#define PASSWORD '2'
#define PREVILAGE '3'

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


