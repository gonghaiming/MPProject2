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

/*序列化模块中使用的常量*/
#define HEADOFFSET 20 /*UDP数据包正文偏移*/
#define TOTALLENGTHOFFSET 2 /*包长度偏移地址*/
#define CONTENTHEADLEN 2
#define CONTENTTYPEOFFSET 1
#define USERNAME '1'
#define PASSWORD '2'
#define PREVILAGE '3'
#define AUTHLENTH 16 /*校验码长度*/
#define ID 1 /*报文中的ID项默认值*/
#define PKT_REQUEST 1
#define PKT_ACCESS 2
#define PKT_REJECT 3

/*通用数据类型定义*/
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
void mini_encrypt(char *passWord,char *hash);

#endif
