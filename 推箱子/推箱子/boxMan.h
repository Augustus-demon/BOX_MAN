#pragma once
#ifndef __BOXMAN_H__
#define __BOXMAN_H__

#define ROW 9
#define COLUMN 12

#define GRAPH_WIDTH 790  
#define GRAPH_HEIGHT 608

#define RATIO 61

#define X_START 30
#define Y_START 30

#define KB_UP 'w'
#define KB_DOWN 's'
#define KB_LEFT 'a'
#define KB_RIGHT 'd'
#define KB_EXIT 'q'

#define isValid(posi) (posi.x >= 0) && (posi.x < ROW) && (posi.y >= 0) && (posi.y < COLUMN)

enum _ISO {
	BOX,
	DES,
	FLOOR,
	MAN,
	WALL,
	HIT,
	ALL
};

struct _character {
	int x; 
	int y;
};

enum _DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};


IMAGE images[ALL];
struct _character man;

#endif