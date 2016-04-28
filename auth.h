#ifndef __AUTH_H
#define __AUTH_H
#include "common.h"
#include <mysql.h>

MYSQL *connection;
MYSQL_RES *result;
MYSQL_ROW row;

void connectSQL(MYSQL **connect);
INT8 authentication(const INT8 *userName,const INT8 *password);

#endif

