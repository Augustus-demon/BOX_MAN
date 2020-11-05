#include <stdlib.h>
#include <iostream>
#include <string>
//#include <graphics.h>
#include <conio.h>

#include "boxMan.h"
#include "database.h"

using namespace std;


int map[ROW][COLUMN] = { 0 };
/*int map[ROW][COLUMN] = {
{WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL },
{WALL ,FLOOR,WALL ,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,WALL ,WALL },
{WALL ,FLOOR,BOX  ,FLOOR,WALL ,DES  ,FLOOR,WALL ,DES  ,FLOOR,WALL ,WALL },
{WALL ,FLOOR,WALL ,FLOOR,WALL ,FLOOR,WALL ,WALL ,FLOOR,FLOOR,FLOOR,WALL },
{WALL ,FLOOR,WALL ,DES  ,WALL ,FLOOR,FLOOR,BOX  ,FLOOR,FLOOR,FLOOR,WALL },
{WALL ,FLOOR,FLOOR,FLOOR,WALL ,MAN  ,FLOOR,FLOOR,FLOOR,BOX  ,FLOOR,WALL },
{WALL ,FLOOR,DES  ,FLOOR,FLOOR,BOX  ,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,WALL },
{WALL ,FLOOR,WALL ,WALL ,FLOOR,WALL ,FLOOR,FLOOR,WALL ,WALL ,FLOOR,WALL },
{WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL }
};*/

/****************************************
* 改变地图数组的内容以及显示改变后的图标
* 输入： 要修改的地点的结构体指针
* 输出： 无
*****************************************/
void changemap(struct _character * xman, enum _ISO iso) {
	map[xman->x][xman->y] = iso;
	putimage(X_START + xman->y * RATIO, Y_START + xman->x * RATIO, &images[iso]);
}
/****************************************
* 小人的移动
* 输入： 移动方向（枚举类型）
* 输出： 无
*****************************************/
void gameControl(enum _DIRECTION direct){
	struct _character next_pos = man;//用临时结构体变量来存储下一位置
	struct _character next_next_pos = man;
	switch (direct) {
		case UP:
			next_pos.x--;
			next_next_pos.x -= 2;
			break;
		case DOWN:
			next_pos.x++;
			next_next_pos.x += 2;
			break;
		case LEFT:
			next_pos.y--;
			next_next_pos.y -= 2;
			break;
		case RIGHT:
			next_pos.y++;
			next_next_pos.y += 2;
			break;
	}
	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR) {
		if (map[man.x][man.y] == DES) {
			changemap(&next_pos, MAN);
			putimage(X_START + man.y * RATIO, Y_START + man.x * RATIO, &images[DES]);
			man = next_pos;
		}
		else {
			if (map[man.x][man.y] == MAN) {
				changemap(&next_pos, MAN);
				changemap(&man, FLOOR);
				man = next_pos;
			}
		}
	}
	else if (isValid(next_next_pos) && (map[next_pos.x][next_pos.y] == BOX)) {
		if (map[man.x][man.y] != DES) {
			if ((map[next_next_pos.x][next_next_pos.y] == FLOOR)) {
				changemap(&next_next_pos, BOX);
				changemap(&next_pos, MAN);
				changemap(&man, FLOOR);
				man = next_pos;
			}
			else if (map[next_next_pos.x][next_next_pos.y] == DES) {
				changemap(&next_next_pos, HIT);
				changemap(&next_pos, MAN);
				changemap(&man, FLOOR);
				man = next_pos;
			}
			
		}
		else {
			changemap(&next_pos, MAN);
			putimage(X_START + man.y * RATIO, Y_START + man.x * RATIO, &images[DES]);
			changemap(&next_next_pos, BOX);
			man = next_pos;
		}
	}
	else if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == DES)) {
		putimage(X_START + next_pos.y * RATIO, Y_START + next_pos.x * RATIO,&images[MAN]);
		changemap(&man, FLOOR);
		man = next_pos;
	}
}
/****************************************
* 判断游戏是否结束
* 输入： 无
* 输出： 无
*****************************************/
bool Gameover() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == DES)
				return false;
		}
	}
	return true;
}
/****************************************
* 游戏结束场景
* 输入： 显示的背景图片
* 输出： 无
*****************************************/
void GameNextScene(IMAGE *image) {
	putimage(0, 0, image);
	settextcolor(RED);
	RECT re = { 0,0,GRAPH_WIDTH ,GRAPH_HEIGHT };
	settextstyle(20,10,"微软雅黑");
	drawtext("Congratulations! click any key to go to next scene.", &re, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	system("pause");
	cleardevice();
}

void GameOverScene(IMAGE* image) {
	putimage(0, 0, image);
	settextcolor(RED);
	RECT re = { 0,0,GRAPH_WIDTH ,GRAPH_HEIGHT };
	settextstyle(20, 10, "微软雅黑");
	drawtext("Congratulations! All of the games are finished !", &re, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	system("pause");
}

//用户登录函数
bool login(userinfo &user) {
	int times = 0;
	bool ret = false;
	
	
	do {

		if (times >= MAX_RETRY_TIME) {
			break;
		}
		//输入用户名密码
		cout << "请输入用户名: ";
		cin >> user.username;

		cout << "请输入密码: ";
		cin >> user.pwd;

		ret = fetch_userinfo(user);
		if (ret == false) {
			times++;
			cout << "登陆失败，请检查密码并重新登录。" << endl;
		}
	} while (!ret);

	return ret;
}

void initGameGraph(IMAGE& bg_Image) {

	//初始化界面
	initgraph(GRAPH_WIDTH, GRAPH_HEIGHT);
	//加载背景
	loadimage(&bg_Image, "blackground.bmp", GRAPH_WIDTH, GRAPH_HEIGHT, true);
	putimage(0, 0, &bg_Image);
	//加载图标
	loadimage(&images[BOX], "box.bmp", RATIO, RATIO, true);
	loadimage(&images[DES], "des.bmp", RATIO, RATIO, true);
	loadimage(&images[FLOOR], "floor.bmp", RATIO, RATIO, true);
	loadimage(&images[MAN], "man.bmp", RATIO, RATIO, true);
	loadimage(&images[WALL], "wall.bmp", RATIO, RATIO, true);
	loadimage(&images[HIT], "box.bmp", RATIO, RATIO, true);
}


/****************************************
* 游戏函数主体
* 输入： 无
* 输出： 无
*****************************************/
void Game() {

	IMAGE bg_Image;

	bool ret = false;
	bool quit = false;

	userinfo user;

	
	//登录
	if (!login(user)) {
		cout << "登陆尝试次数过多，请重新登录。" << endl;
		exit(-1);
	}
	else {
		cout << "登陆成功，游戏开始。PS:user_id:" << user.id << ",level_id:" << user.level_id << endl;
		Sleep(1000);
	}
	


	do {

		
		/*ret = fetch_userinfo(user);
		if (ret == false) {
			cout << "获取用户信息失败。" << endl;
			exit(-2);
		}*/

		initGameGraph(bg_Image);
		//获取地图信息
		levelinfo level;

		ret = fetch_map_data(level, user.level_id);
		if (!ret) {
			cout << "获取关卡信息失败" << endl;
			system("pause");
			exit(-3);
		}


		ret = transform_data_to_array(level, map);
		if (!ret) {
			cout << "转换关卡信息失败" << endl;
			system("pause");
			exit(-4);
		}




		//初始化地图元素
		for (int i = 0; i < level.map_row; i++) {
			for (int j = 0; j < level.map_column; j++) {
				if (map[i][j] == MAN) {
					man.x = i;
					man.y = j;
				}
				putimage(X_START + j * RATIO, Y_START + i * RATIO, &images[map[i][j]]);
			}
		}
		//游戏开始！
		
		do{
			if (_kbhit()) {
				//玩家按键
				char ch = _getch();
				switch (ch) {
				case KB_EXIT:
					quit = true;
					break;
				case KB_UP:
					gameControl(UP);
					break;
				case KB_DOWN:
					gameControl(DOWN);
					break;
				case KB_LEFT:
					gameControl(LEFT);
					break;
				case KB_RIGHT:
					gameControl(RIGHT);
					break;
				}
				//判断游戏是否结束
				if (Gameover()) {

					if (level.next_level < 1) {
						GameOverScene(&bg_Image);
						quit = true;
						break;
					}

					if (update_user_level(user, level.next_level)) {
						user.level_id = level.next_level;
					}

					GameNextScene(&bg_Image);
					break;
					//quit = true;
				}
				//不放在if外面是为了避免CPU资源浪费，只有有键盘输入才有可能结束游戏
			}
			Sleep(100);
		} while (quit == false);
	} while (quit == false);
	closegraph();
}
int main(void) {
	Game();
	return 0;
}