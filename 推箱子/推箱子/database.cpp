#include <mysql.h>

#include "database.h"


#define DB_HOST "127.0.0.1"
#define DB_USERNAME "root"
#define DB_PWD "13033178940"
#define DB_NAME "box_man"
#define DB_PORT 3306

static const int debug = 0;

bool connect_to_db(MYSQL& mysql);

/************************************
*功能: 根据输入的用户名密码从用户表里获取用户信息
*输入:
		用户信息结构体
*返回: 获取成功返回true，失败返回false
**************************************/
bool fetch_userinfo(userinfo& user) {
	
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char buf[256] = { 0 };
	bool ret = false;

	//连接到数据库
	if (connect_to_db(mysql) == false) {
		return false;
	}

	//根据用户名和密码查询用户信息
	snprintf(buf, 256, "select id,level_id from users where username = '%s' and password = md5('%s');", user.username.c_str(), user.pwd.c_str());

	ret = mysql_query(&mysql, buf);
	if (ret) {
		printf("数据库查询%s出错，错误原因:%s\n", buf, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}
	
	//获取查询结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);

	if(debug) printf("用户id: %s,用户当前关卡:%s\n", row[0], row[1]);
	//返回结果并清理资源
	mysql_free_result(res);
	mysql_close(&mysql);

	return true;

}
/************************************
*功能: 根据输入的用户的关卡ID获取关卡信息
*输入:
		关卡信息结构体，用户所在关卡ID
*返回: 获取成功返回true，失败返回false
**************************************/
bool fetch_map_data(levelinfo& level, int level_id) {
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char buf[256] = { 0 };
	bool ret = false;

	//连接到数据库
	if (connect_to_db(mysql) == false) {
		return false;
	}

	//根据用户名和密码查询用户信息
	snprintf(buf, 256, "select name,map_row,map_column,map_data,next_level_id from levels where id = %d;", level_id);

	ret = mysql_query(&mysql, buf);
	if (ret) {
		printf("数据库查询%s出错，错误原因:%s\n", buf, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	//获取查询结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}
	level.id = level_id;
	level.map_name = row[0];
	level.map_row = atoi(row[1]);
	level.map_column = atoi(row[2]);
	level.map_data = row[3];
	level.next_level = atoi(row[4]);

	if (debug) {
		printf("level id: %d  name: %s map row: %d  map column: %d map data: %s next level: %d\n",
			level.id, level.map_name.c_str(), level.map_row, level.map_column, level.map_data.c_str(), level.next_level);
		system("pause");
	}

	//返回结果并清理资源
	mysql_free_result(res);
	mysql_close(&mysql);
	return true;
}
/************************************
*功能: 连接到数据库
*输入:
		数据库连接会话句柄
*返回: 连接成功返回true，失败返回false
**************************************/
bool connect_to_db(MYSQL& mysql) {
	//1.初始化句柄
	mysql_init(&mysql);
	

	//2.设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	//3.登录到数据库
	if (mysql_real_connect(&mysql, DB_HOST, DB_USERNAME, DB_PWD, DB_NAME, DB_PORT, NULL, 0) == NULL) {
		printf("数据库连接失败，失败原因:%s\n", mysql_error(&mysql));
		return false;
	}
	
	return true;
}

bool update_user_level(userinfo& user, int next_level_id) {
	MYSQL mysql;
	char buf[256] = { 0 };
	bool ret = false;

	//连接到数据库
	if (connect_to_db(mysql) == false) {
		return false;
	}

	//更新用户的关卡信息
	snprintf(buf, 256, "update users set level_id = %d where id = %d;", next_level_id,user.id);

	ret = mysql_query(&mysql, buf);
	if (ret) {
		printf("数据库更新%s出错，错误原因:%s\n", buf, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	//返回结果并清理资源
	mysql_close(&mysql);

	return true;
}

/************************************
*功能: 解析得到的地图数据
*输入:
		关卡信息和底图数组
*返回: 解析成功返回true，失败返回false
**************************************/
bool transform_data_to_array(levelinfo& level, int map[ROW][COLUMN]) {
	if (level.map_row > ROW || level.map_column > COLUMN) {
		printf("地图设置过大,请重新设计。\n");
		return false;
	}

	if (level.map_column < 1) {
		printf("地图数据为空，终止解析。\n");
		return false;
	}

	int start = 0, end = 0;
	int row = 0, column = 0;
	do {
		end = (int)level.map_data.find('|', start);
		if (end < 0) {
			end = (int)(level.map_data.length());
		}

		if (start > end)break;

		std::string line = level.map_data.substr(start, end - start);

		if (debug)printf("get line:%s\n", line.c_str());

		char* next_token = NULL;
		char* item = strtok_s((char*)line.c_str(), ",", &next_token);

		column = 0;
		if (debug) printf("         ");
		while (item && column < level.map_column) {
			if (debug) printf("%s ", item);
			map[row][column] = atoi(item);
			column++;
			
			item = strtok_s(NULL, ",", &next_token);
		}

		if (column < level.map_column) {
			printf("地图解析出错，终止！\n");
			return false;
		}

		row++;
		if (row >= level.map_row) {
			break;
		}
		if (debug) printf("\n");

		
		start = end + 1;
	} while (true);

	if (row < level.map_row) {
		printf("地图行数少于设定值,%d/%d\n",row,level.map_row);
		return false;
	}

	return true;
}
