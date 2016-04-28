#include <stdio.h>
#include "common.h"
#include <string.h>
#include <arpa/inet.h>

#define HEADOFFSET 20
/*****************************************************************
* DESCRIPTION:
*
* INPUTS:
*       pktdata-UPD数据包
* OUTPUTS:
*       进行校验比对的密码hash值
* RETURNS:
*       进行校验比对的密码hash值指针
*****************************************************************/

#define TOTALLENGTHOFFSET 2
void deserialized(char *pktdata,userInfo *pUserInfo){
    
    INT16 * len = (INT16 *)(pktdata + 2);
    (*len) = ntohs(* (INT16 *)len);
    
    
    INT8 *pos  = pktdata;
    INT16 pktlength = *(INT16 *)(pos + TOTALLENGTHOFFSET);
    INT8 *pktTail = pos + pktlength;
    
    pos = pos + sizeof(struct pktHeader);
    
    while(pos != pktTail)
    {
        INT8 type = (*pos);
        INT8 contentLength = * (INT8 *)(pos + 1);
        switch (type) {
            case '1':
                memcpy(pUserInfo->name, pos + 2, contentLength - 2);
                pos += contentLength;
                break;
            case '2':
                memcpy(pUserInfo->password, pos + 2, contentLength - 2);
                pos += contentLength;
                break;
            case '3':
                pUserInfo-> previlage = * (INT8 *)(pos + 2);
                pos = pos + contentLength;
                break;
            default:
                printf("cannot get the correct content from the pakage");
                break;
        }
        
    }
    return;
}


