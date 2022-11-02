#pragma once

#include<windows.h>
#ifndef __STDIO_H__
#define __STDIO_H__
	#include<stdio.h>
#endif
#ifndef __STDLIB_H__
#define __STDLIB_H__
	#include<stdlib.h>
#endif
#ifndef __CONIO_H__
#define __CONIO_H__
	#include<conio.h>
#endif
#ifndef __TIME_H__
#define __TIME_H__
	#include<time.h>
#endif

#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_DIRECTION 224
#define KEY_LEFT 75  
#define KEY_RIGHT 77 
#define KEY_UP 72  
#define KEY_DOWN 80 

#define KEY_p 'p'
#define KEY_P 'P'

#define VK_P 0x50

#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UP 3
#define DIR_DOWN 4

#define DEFAULTPOS_X 19
#define DEFAULTPOS_Y 19

#define DATA_EMPTY 0
#define DATA_BULLET 1

#define OFFSET_TITLE_X 5
#define OFFSET_TITLE_Y 4
#define OFFSET_GAME_X 5
#define OFFSET_GAME_Y 5

#define SIZE_GAMEMAP 40
#define SIZE_WALL 39

#define UI_POS_END_X 5
#define UI_POS_END_Y 5
#define UI_SIZE_END_X 30
#define UI_SIZE_END_Y 15

#define UI_POS_PAUSE_X 5
#define UI_POS_PAUSE_Y 0
#define UI_POS_HP_X 23
#define UI_POS_HP_Y 47
#define UI_END_POS_X 14
#define UI_END_POS_Y 10
#define UI_END_SIZE_X 22
#define UI_END_SIZE_Y 10

#define DRAW_EMPTY "  "
#define DRAW_GAME_WALL "  "
#define DRAW_UI_FRAME "¢Ã"
#define DRAW_UI_END_FRAME "¢Ã"
#define DRAW_BULLET "¡Ú"
#define DRAW_PLAYER "¡ã"
#define DRAW_HP "¡á"

#define GAME_BG_COLOR COLOR_BLACK
#define WALL_COLOR COLOR_WHITE
#define WARNING_COLOR COLOR_RED
#define BULLET_COLOR COLOR_RED
#define PLAYER_COLOR COLOR_YELLOW
#define HP_BG_COLOR COLOR_DARK_RED
#define HP_COLOR COLOR_GREEN

#define DEFAULT_BULLET_CREATE_SPD 150
#define DEFAULT_BULLET_MOVE_SPD 60
#define HP_MAX 5
#define LEVEL_CNT 500
#define SCORE_CNT 100
#define SCORE_UNIT 10
#define BULLET_CREATE_SPD_UNIT 1.5
#define BULLET_MOVE_SPD_UNIT 1

#define BULLET_W_CNT 750

#define UPDATE_UNIT 10

enum
{
	COLOR_BLACK = 0,
	COLOR_WHITE = 15,
	COLOR_BLUE = 9,
	COLOR_SKYBLUE = 11,
	COLOR_RED = 12,
	COLOR_DARK_RED = 4,
	COLOR_ORANGE = 6,
	COLOR_YELLOW = 14,
	COLOR_PURLPE = 13,
	COLOR_GREEN = 10,
	COLOR_GRAY = 7,
	COLOR_DARK_GRAY = 8
};
enum
{
	WALL_LEFT,
	WALL_RIGHT,
	WALL_UP,
	WALL_DOWN,
	WALL_DIR_MAX
};
enum
{
	BULLET_NULL = 0,
	BULLET_UP,
	BULLET_UP_R,
	BULLET_RIGHT,
	BULLET_DOWN_R,
	BULLET_DOWN,
	BULLET_DOWN_L,
	BULLET_LEFT,
	BULLET_UP_L,
	BULLET_MAX
};
typedef struct _BULLET
{
	COORD pos;
	COORD trace;
	int Dir;
	int wCnt;
	float fMoveTime;
	BOOL isActive;
}BULLET;
typedef struct _NODE
{
	BULLET bullet;
	_NODE* before;
	_NODE* after;
}NODE;

void InitHeadTail(void);
void InsertNode(BULLET bullet);
void DeleteNode(NODE * deleteNode);
void DeleteAllNode(void);

void SetTextColor(int bgColor, int textColor);
void GotoxyDraw(int x, int y, const char* str);
void Gotoxy(int x, int y);
void GotoxyDrawC(int x, int y, const char c);
void SetNoCursor(void);
void Title(void);
void Game(void);
void Exit(void);
void Init(void);
void DrawWall(void);
void CreateBullet(void);
void MoveBulletData(BULLET* pBullet);
void KeyControl(void);
BOOL CheckColWall(int x, int y);
void UpdateHpUI(void);
BOOL BulletProcessing(void);
void UpdateSubUI(void);
void UpdateScoreUI(void);
void ShowGameEnd(void);