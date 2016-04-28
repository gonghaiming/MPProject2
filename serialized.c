
#include "common.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#define DEBUG
/*****************************************************************
 * DESCRIPTION:
 *
 * INPUTS:
 *        pktdata-UPD网络数据包（大端）
 *        package UPD数据包结构体数据
 * OUTPUTS:
 *        pktdata-网络流后的序列化数据
 * RETURNS:
 *        NULL
 *****************************************************************/
INT16 clientSerialized(char *pktdata,pktcontent *pcontent)
{
    
    pktHeader header;
    INT8 * pos = pktdata;
    
    header.code = 1;
    header.id = 1;
    memset(header.md5Auth, 0, 16);
    
    INT16 TotalVLength = 0;
    
    for( int i = 0; i < 2 ; i++){
        TotalVLength += pcontent[i].lengthItem;
    }
    
    header.lengthPkt = TotalVLength + 20;
    
    header.lengthPkt = htons(header.lengthPkt);
    
    memcpy(pktdata, &header, 20);
    pos += 20;
    for( int i = 0; i < 2 ; i++){
        memcpy(pos,pcontent+i,pcontent[i].lengthItem);
        pos += pcontent[i].lengthItem;
    }
    
    
    
    
    return TotalVLength + 20;
    
}

INT16 serverSerialized(char *pktdata,pktcontent *pcontent,int code){
    pktHeader header;
    INT8 *pos = pktdata;
    INT8 type  = pcontent->type;
     header.code = code;
     header.id = 1;
    memset(header.md5Auth, 0, 16);
    switch (type) {
        case '2':
            header.lengthPkt = pcontent->lengthItem + 20;
            header.lengthPkt = htons(header.lengthPkt);
            memcpy(pktdata, &header, 20);
            pos += 20;
            memcpy(pos, pcontent, pcontent->lengthItem);
            
            break;
        case '3':
            header.lengthPkt = pcontent->lengthItem + 20;
            header.lengthPkt = htons(header.lengthPkt);
            memcpy(pktdata, &header, 20);
            pos += 20;
            memcpy(pos, pcontent, pcontent->lengthItem);
            break;
            
        default:
            printf("erro header type");
            break;
    }
    
    
    return 20 + pcontent->lengthItem;
    
}

