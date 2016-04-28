//
//  main.c
//  mysqlconnect
//
//  Created by MR_gong on 16/4/28.
//  Copyright © 2016年 gong. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include "common.h"
#include "auth.h"

int main(int argc, const char * argv[]) {

    
    
    char *username = "hello";
    char *password = "123";
    
    int retValue;
    retValue = authentication(username, password);
    assert(retValue != 1);
    
    
    
    
    username = "world";
    password = "000000";
    retValue = authentication(username, password);
    assert(retValue == 2);
    
    username = "world";
    password = "00";
    retValue = authentication(username, password);
    assert(retValue == 0);
    
    username = "wo123";
    password = "00";
    retValue = authentication(username, password);
    assert(retValue == 0);
    
    printf("数据库认证模块单元测试用例通过\n");

    
    return 0;
}
