/*****************************************************************
 * Copyright (C) 2010 Maipu Communication Technology Co.,Ltd.*
 ******************************************************************
 * server.C
 *
 * DESCRIPTION:
 * 客户端监听端口，接收到报文后开始解析；
 * 链接书库认证信息，返回用户认证情况和权限；
 *
 * AUTHOR:
 * MR_GONG、MR_ZHAO、MR_HU
 *
 * CREATED DATE:
 * 2016.4.28
 *
 * REVISION:
 * 1.0
 *
 * MODIFICATION HISTORY
 * --------------------
 * $Log:$
 * *****************************************************************/

#include "common.h"
#include "auth.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <mysql.h>

#define PREVILAGECONTENTLENGTH 3

#define PORT 8888
#define BUFSIZE 2000	/*定义接收缓冲区大小*/


INT32 main(INT32 argc, INT8 *argv[])
{
	INT32 sockFd;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	INT8 recvBuf[BUFSIZE];	/*接收缓冲区*/
	INT32 addrLen;
	INT32 recvSize;	/*接收到的报文大小*/

	userInfo pUserInfo;
    //MYSQL *conn;
    //connectSQL(&conn);
	connectSQL(&connection);
    //authentication("hello", "111");
	authentication("hello", "000000");
	
	/*套接字*/
	sockFd = socket(AF_INET, SOCK_DGRAM, 0);

	/*初始化套接字*/
	bzero(&serverAddr, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*绑定端口*/
	bind(sockFd, (struct sockaddr *)(&serverAddr), sizeof(struct sockaddr));

	/*开始接收数据*/
	while (1)
	{
		addrLen = sizeof(struct sockaddr);
		bzero(recvBuf, sizeof(recvBuf));
		
        /*接收数据*/
		recvSize = recvfrom(sockFd, recvBuf, BUFSIZE, 0, (struct sockaddr *)(&clientAddr), &addrLen);
		recvBuf[recvSize] = 0;
		printf("recv: %s\n", recvBuf);
		
        /*用户信息的分析和认证*/
		INT8 retValue;
		INT8 code;
		deserialized(recvBuf, &pUserInfo);
		retValue = authentication(pUserInfo.name, pUserInfo.password);
		if (retValue == '0')
		{
			code = PKT_REJECT;
		}
		else
		{
			code = PKT_ACCESS;
		}
		
        /*回应报文*/
		pktcontent pcontent;
		pcontent.type = PREVILAGE;
		pcontent.lengthItem = PREVILAGECONTENTLENGTH;
		pcontent.value[0] = retValue;
		
		bzero(recvBuf, sizeof(recvBuf));
		INT16 pkgLen = serverSerialized(recvBuf, &pcontent, code);

		/*返送回客户端*/
		sendto(sockFd, recvBuf, pkgLen, 0, (struct sockaddr *)(&clientAddr), addrLen);
	}
    
	close(sockFd);
	mysql_close(connection);

	return 0;
}
