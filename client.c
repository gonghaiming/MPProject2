/*****************************************************************
 * Copyright (C) 2010 Maipu Communication Technology Co.,Ltd.*
 ******************************************************************
 * client.c
 *
 * DESCRIPTION:
 * 客户端键入用户名和密码，处理用户名和密码报文的封装；
 * 等待服务器回应，超时自动重连，五次重连后放弃；
 * 连接成功或失败回应用户权限；
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFSIZE 2000

INT32 flag = 0;

#if 0
void *countTime(void* ptr)
{
    while (1)
    {
        //sleep(5000);
	if (flag == 0)
		printf("等待超时！\n");
    }
   
    return NULL;
}
#endif

INT32 main(INT32 argc, INT8 *argv[])
{
	INT32 threadRe;
	INT32 ret;

    INT32 addrLen ;
 
	INT32 sockFd;
	INT8 sendBuf[BUFSIZE];
	INT8 passWord[BUFSIZE];
	struct sockaddr_in serverAddr;
    
/*检验是否键入服务器地址*/
#if 0
	if (argc != 2)
	{
		printf("未输入服务器地址！");
		return 0;
	}
#endif	
    
    /*创建套接字*/
	sockFd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&serverAddr, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


//	while(1)
	{
		printf("name:");

		bzero(&sendBuf, strlen(sendBuf));/*清除缓冲区*/

		bzero(&passWord, BUFSIZE);

		fgets(sendBuf, BUFSIZE, stdin);	/*读取用户名*/

		INT32 nameLen = strlen(sendBuf);
		sendBuf[nameLen-1] = '\0';

		fgets(passWord, BUFSIZE, stdin); /*读取密码*/
		INT32 pwdLen = strlen(passWord);
		passWord[pwdLen-1] = '\0';
		

		INT8 hash[33] = {'\0'};
		encrypt(passWord, hash);
        
        /*封装报文*/
		pktcontent pcontent[2] = {{'1', nameLen + 2}, {'2', 34}};
		strcpy(pcontent[0].value, sendBuf);
		strcpy(pcontent[1].value, hash);
		bzero(&sendBuf, strlen(sendBuf));
		INT16 pktLen = clientSerialized(sendBuf, pcontent);
		
		for (int i =0 ; i < pktLen; ++i)
			printf("string: %x\n", sendBuf[i]);
        
        /*发送报文*/
		sendto(sockFd, sendBuf, pktLen, 0, (struct sockaddr *)(&serverAddr), sizeof(struct sockaddr));
		bzero(sendBuf, BUFSIZE);


		//ret = pthread_create(&threadRe, NULL, countTime, NULL);
        
        
        /*接收回应报文*/
		INT32 sendSize;
		addrLen = sizeof(struct sockaddr);
		sendSize = recvfrom(sockFd, sendBuf, BUFSIZE, 0, (struct sockaddr *)(&serverAddr), &addrLen);
                sendBuf[sendSize] = 0;
		for (int i =0 ; i < pktLen; ++i)
			printf("string: %x\n", sendBuf[i]);

		userInfo pUserInfo;
		deserialized(sendBuf, &pUserInfo);
                printf("权限为: %c\n", pUserInfo.previlage);
	}
	
	close(sockFd);
	return 0;
}
