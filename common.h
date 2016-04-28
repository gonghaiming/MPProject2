/*****************************************************************
 * Copyright (C) 2010 Maipu Communication Technology Co.,Ltd.*
 ******************************************************************
 * commmon.h
 *
 * DESCRIPTION:
 * 报文结构和数据类型头文件
 * AUTHOR:
 * MR_GONG
 * CREATED DATE:
 * 2016-4.27
 * REVISION:
 * 1.0
 *
 * MODIFICATION HISTORY
 * --------------------
 * $Log:$
 * *****************************************************************/

#ifndef __COMMON_H
#define __COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



typedef char INT8; 
typedef short INT16; 
typedef int INT32; 
typedef unsigned char UCHAR; 
typedef signed char CHAR; 
typedef unsigned short UINT16; 
typedef unsigned int UINT32; 
typedef signed int INT32; 
typedef signed long long INT64; 
typedef unsigned long long UINT64; 
typedef int BOOL;

/*报文头部*/
typedef struct pktHeader
{
    INT8 code;
    INT8 id;
    INT16 lengthPkt;
    INT8 md5Auth[16];
}pktHeader;

/*报文正文内容*/
typedef struct content
{
    INT8 type;
    INT8 lengthItem;
    INT8 value[33];
}pktcontent;

/*报文*/
typedef struct pkt
{
    struct pktHeader head;
    INT8 content[1500];
}pkt;

/*用户信息*/
typedef struct userInfo{
	INT8 name[20];
	INT8 password[32];
    INT8 previlage;
}userInfo;

void deserialized(char *pktdata,userInfo *pUserInfo);
INT16 clientSerialized(char *pktdata,pktcontent *pcontent);
INT16 serverSerialized(char *pktdata,pktcontent *pcontent, int code);
void encrypt(char *passWord,char *hash);

#endif
