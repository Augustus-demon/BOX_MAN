#pragma once

#include <string>

#define ROW 48
#define COLUMN 48

typedef struct _userinfo {
	int id;
	std::string username;
	std::string pwd;
	int level_id;

	_userinfo() :id(0), username(""), pwd(""), level_id(1) {}
}userinfo;

typedef struct _levelinfo {
	int id;
	std::string map_name;
	int map_row;
	int map_column;
	std::string map_data;
	int next_level;
}levelinfo;

bool fetch_userinfo(userinfo& user);

bool update_user_level(userinfo &user, int next_level_id);

bool fetch_map_data(levelinfo &level, int level_id);
bool transform_data_to_array(levelinfo& level, int map[ROW][COLUMN]);
