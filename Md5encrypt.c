
/*****************************************************************
 * Copyright (C) 2010 Maipu Communication Technology Co.,Ltd.*
 ******************************************************************
 * MD5encrypt.c
 *
 * DESCRIPTION:
 *         加密模块
 * AUTHOR:
 *         mr_zhao,mr_gong,mr_hu
 * CREATED DATE:
 *         2016.4.28
 * REVISION:
 * 1.0
 *
 * MODIFICATION HISTORY
 * --------------------
 * $Log:$
 * *****************************************************************/


#include "common.h"
#include <openssl/md5.h>

#define MD5LENGTH 16

/*****************************************************************
 * DESCRIPTION:
 *       对报文的加密函数模块
 * INPUTS:
 *       报文中的密码
 * OUTPUTS:
 *       进行加密的密码hash值
 * RETURNS:
 *       NULL
 *****************************************************************/

void encrypt
(
 INT8 *passWord,
 INT8 *hash)
{
	
	MD5_CTX ctx;
	UCHAR md5[16];
	INT8 md5x[33] = {'\0'};	/*十六进制MD5值*/
	INT8 tmp[3] = {'\0'};
	INT32 transNum = 0;

    bzero(&md5x, 32);
    bzero(&tmp, 2);
    transNum = 0;
    
        /*计算MD5值*/
    MD5_Init(&ctx);
    MD5_Update(&ctx, passWord, strlen(passWord));
    MD5_Final(md5, &ctx);
    
    printf("length:%d\n", (int)strlen(passWord));
    printf("md5:%s\n", md5);
    
    for(; transNum < MD5LENGTH; ++transNum)
    {
        sprintf(tmp, "%02x", md5[transNum]);
        strcat(md5x, tmp);
    }
    
    printf("md5x:%s\n", md5x);
	strcpy(hash, md5x);

	return;
}
