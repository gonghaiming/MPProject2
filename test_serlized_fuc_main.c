//
//  main.c
//  testSerilized
//
//  Created by MR_gong on 16/4/27.
//  Copyright © 2016年 gong. All rights reserved.
//

#include <stdio.h>
#include "common.h"
#include <string.h>


int main(int argc, const char * argv[]) {
    pktcontent content[2];
    
    content[0].type = 1;
    content[0].lengthItem = 8;
    content[0].value[0] = 'a';
    content[0].value[1] = 'b';
    content[0].value[2] = 'c';
    content[0].value[3] = 'd';
    content[0].value[4] = 'e';
    content[0].value[5] = 'f';
    
    content[1].type = 2;
    content[1].lengthItem = 12;
    content[1].value[0] = 'a';
    content[1].value[1] = 'b';
    content[1].value[2] = 'c';
    content[1].value[3] = 'd';
    content[1].value[4] = 'e';
    content[1].value[5] = 'f';
    content[1].value[6] = 'g';
    content[1].value[7] = 'h';
    content[1].value[8] = 'a';
    content[1].value[9] = 'd';
    
    int length = 0;
    
    char buff[1500];
    memset(buff, 0, 1500);
    
    length = clientSerialized(buff, content);
    
    printf("%s ",buff);
    
    printf("pktLength = %d\n",length);
    
    userInfo userinfo;
    userinfo.previlage = 1;
    deserialized(buff, &userinfo);
    printf("username = %s\n",userinfo.name);
    printf("password = %s\n",userinfo.password);
    
    
    
    
    
    
    
    
    
    
    
    
    // 
    


    
    return 0;
}
