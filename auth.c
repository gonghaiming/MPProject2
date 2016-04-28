#include "auth.h"

/*****************************************************************
* DESCRIPTION:
*       服务器连接数据库函数模块
* INPUTS:
*       数据库接口
* OUTPUTS:
*       NULL
* RETURNS:
*       NULL
*****************************************************************/

void connectSQL(MYSQL **connect)
{
	INT8 server[] = "localhost";
	INT8 user[] = "jia";
	INT8 password[] = "1";
	INT8 database[] = "auth";

	*connect = mysql_init(NULL);
    //mysql_init(*connect);

	if (!mysql_real_connect(*connect, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(*connect));
		exit(1);
	}

	return ;
}

/*****************************************************************
 * DESCRIPTION:
 *       认证用户信息函数模块
 * INPUTS:
 *       用户名和密码
 * OUTPUTS:
 *       认证信息和权限
 * RETURNS:
 *       认证信息和权限
 *****************************************************************/

INT8 authentication(const INT8 *userName, const INT8 *passWord)
{
	INT32 level; /*存放用户权限*/
    
	/*查询数据库*/
	INT8 query[1024] = {'\0'};
	strcat(query, "select * from userinfo where username=\"");
	strcat(query, userName);
	strcat(query, "\"");
    //printf("query sentence: %s\n", query);
    
	if (mysql_query(connection, query))
	{
		fprintf(stderr, "%s\n", mysql_error(connection));
		exit(1);
	}

	result = mysql_use_result(connection);

	INT8 passWd[33] = {'\0'};
	while ((row = mysql_fetch_row(result)) != NULL)
	{
		printf("%s %s %s\n", row[0], row[1], row[2]);
		strcpy(passWd, row[1]);
		level = atoi(row[2]);
	}

	/*开始计算MD5值*/	
	bzero(&query, 1024);
	strcat(query, "select md5(\""); 
	strcat(query, passWd);
	strcat(query, "\")");
    //printf("query sentence: %s\n", query);
    
	if (mysql_query(connection, query))
	{
		fprintf(stderr, "%s\n", mysql_error(connection));
		exit(1);
	}

	MYSQL_RES *resultMD5 = mysql_use_result(connection);	/*得到数据库密码的MD5值*/

	bzero(&passWd, 1024);
	MYSQL_ROW rowMD5;
    
    while ((rowMD5 = mysql_fetch_row(resultMD5)) != NULL)
	{
		printf("%s\n", rowMD5[0]);
		strcpy(passWd, rowMD5[0]);
	}

	mysql_free_result(resultMD5);
	mysql_free_result(result);
	
	/*比较收到的密码与数据库中到密码一致性*/
	if (strcmp(passWord, passWd) == 0)
	{
		printf("认证成功！\n");
		printf("权限：%d\n", level);
		return level+48;
	}
	else
	{
		printf("认证失败！\n");
		return '0';
	}

	return '0';	
}
