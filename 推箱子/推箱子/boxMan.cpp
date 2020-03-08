#include <stdlib.h>
#include <iostream>
#include <string>
#include <graphics.h>
#include <conio.h>

#include "boxMan.h"

using namespace std;

int map[ROW][COLUMN] = {
{WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL },
{WALL ,FLOOR,WALL ,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,WALL ,WALL },
{WALL ,FLOOR,BOX  ,FLOOR,WALL ,DES  ,FLOOR,WALL ,DES  ,FLOOR,WALL ,WALL },
{WALL ,FLOOR,WALL ,FLOOR,WALL ,FLOOR,WALL ,WALL ,FLOOR,FLOOR,FLOOR,WALL },
{WALL ,FLOOR,WALL ,DES  ,WALL ,FLOOR,FLOOR,BOX  ,FLOOR,FLOOR,FLOOR,WALL },
{WALL ,FLOOR,FLOOR,FLOOR,WALL ,MAN  ,FLOOR,FLOOR,FLOOR,BOX  ,FLOOR,WALL },
{WALL ,FLOOR,DES  ,FLOOR,FLOOR,BOX  ,FLOOR,FLOOR,FLOOR,FLOOR,FLOOR,WALL },
{WALL ,FLOOR,WALL ,WALL ,FLOOR,WALL ,FLOOR,FLOOR,WALL ,WALL ,FLOOR,WALL },
{WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL ,WALL }
};
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
				changemap(&next_pos, MAN);//传入指针以节约空间
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
void GameOverScene(IMAGE * image) {
	putimage(0, 0, image);
	settextcolor(RED);
	RECT re = { 0,0,GRAPH_WIDTH ,GRAPH_HEIGHT };
	settextstyle(20,10,"微软雅黑");
	drawtext("Congratulations ! ", &re, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	system("pause");
}
/****************************************
* 游戏函数主体
* 输入： 无
* 输出： 无
*****************************************/
void Game() {
	IMAGE bg_Image;
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
	//初始化地图元素
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == MAN) {
				man.x = i;
				man.y = j;
			}
			putimage(X_START + j * RATIO, Y_START + i * RATIO, &images[map[i][j]]);
		}
	}
	//游戏开始！
	bool quit = false;
	while (quit == false) {
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
				GameOverScene(&bg_Image);
				quit = true;
			}
			//不放在if外面是为了避免CPU资源浪费，只有有键盘输入才有可能结束游戏
		}
		Sleep(100);
	}
	closegraph();
}
int main(void) {
	Game();
	return 0;
}