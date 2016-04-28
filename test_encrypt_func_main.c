//
//  main.c
//  testEncrypt
//
//  Created by MR_gong on 16/4/28.
//  Copyright © 2016年 gong. All rights reserved.
//

#include <stdio.h>
#include "common.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char *password1 = "";
    char hash[33];
    mini_encrypt(password1, hash);
    
    password1 = "13123124";
    mini_encrypt(password1, hash);
    
    password1 = "sldajflk12";
    mini_encrypt(password1, hash);
    
    
    password1 = "1asdf12312";
    mini_encrypt(password1, hash);
    
    password1 = "asdfjadsfj";
    mini_encrypt(password1, hash);
    
    password1 = "chasdjfk";
    mini_encrypt(password1, hash);

    return 0;
}
