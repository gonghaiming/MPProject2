#include "common.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFSIZE 2000

INT32 flag = 0;

#if 0
void *countTime(void* ptr)
{
    while(1)
    {
        //sleep(5000);
	if (flag == 0)
		printf("等待超时！\n");
    }
   
    return NULL;
}
#endif

int main(int argc, char *argv[])
{
	INT32 threadRe;
	INT32 ret;

INT32 addrLen ;
 
	INT32 sockFd;
	INT8 sendBuf[BUFSIZE];
	INT8 passWord[BUFSIZE];
	struct sockaddr_in serverAddr;
#if 0
	if(argc != 2)
	{
		printf("未输入服务器地址！");
		return 0;
	}
#endif	

	sockFd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&serverAddr, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.23.7");
		
		printf("***************************************\n");
		printf(" ^_^                                   \n");
		printf("  Welcome Mini Authentication System!!!\n");
		printf("                written by Gong,Zhao,Hu\n");
		printf(" ^_^                                   \n");
		printf("***************************************\n");

		printf("请输入账号:");

		/*清除缓冲区*/
		bzero(&sendBuf, strlen(sendBuf));

		bzero(&passWord, BUFSIZE);

		fgets(sendBuf, BUFSIZE, stdin);	/*读取用户名*/

		INT32 nameLen = strlen(sendBuf);
		sendBuf[nameLen-1] = '\0';
		
		printf("请输入密码:");
		fgets(passWord, BUFSIZE, stdin); /*读取密码*/
		INT32 pwdLen = strlen(passWord);
		passWord[pwdLen-1] = '\0';
		

		INT8 hash[33] = {'\0'};
		mini_encrypt(passWord, hash);
	
		pktcontent pcontent[2] = {{'1', nameLen + 2}, {'2', 34}};
		strcpy(pcontent[0].value, sendBuf);
		strcpy(pcontent[1].value, hash);
		bzero(&sendBuf, strlen(sendBuf));
		INT16 pktLen = clientSerialized(sendBuf, pcontent);
		
		for(int i =0 ; i < pktLen; ++i)
			printf("%x", sendBuf[i]);
		/*最多发送5次*/
		INT32 circleNum = 0;
		while(circleNum < 5)
		{
		printf("circleNum = %d\n", circleNum);
		circleNum++;
		sendto(sockFd, sendBuf, pktLen, 0, (struct sockaddr *)(&serverAddr), sizeof(struct sockaddr));
		bzero(sendBuf, BUFSIZE);


		//ret = pthread_create(&threadRe, NULL, countTime, NULL);
		/*设置超时检测*/
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		if (setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
		{
			printf("socket option SO_RCVTIMEO not support!\n");
			return -1;
		}
		


		INT32 sendSize;
		addrLen = sizeof(struct sockaddr);
		sendSize = recvfrom(sockFd, sendBuf, BUFSIZE, 0, (struct sockaddr *)(&serverAddr), &addrLen);
		if (sendSize < 0)
		{
			//if(ret == EWOULDBLOCK || ret == EAGAIN)
				printf("recvfrom timeout\n");
			//else	
				printf("recvfrom err:%d\n", ret);
		}
		else
		{
                sendBuf[sendSize] = 0;
		for(int i =0 ; i < pktLen; ++i)
			printf("string: %x\n", sendBuf[i]);

		userInfo pUserInfo;
		deserialized(sendBuf, &pUserInfo);
                printf("权限为: %c\n", pUserInfo.previlage);
		break;
		}
		
		if(circleNum == 5)
			printf("网络不可达！\n");
	}
	
	close(sockFd);
	return 0;
}
