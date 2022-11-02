#include "bullets.h"
NODE head = { 0 };
NODE tail = { 0 };

COORD arrWall[WALL_DIR_MAX][SIZE_WALL] = { 0 };
char arrBulletDir[BULLET_MAX][3] = { 0 };
COORD myPos = { 0 };
float bulletCreateSpd = 0.0f;
float bulletMoveSpd = 0.0f;
int score = 0;
int hp = 0;

void InitHeadTail(void)
{
	head.after = &tail;
	head.before = NULL;
	tail.before = &head;
	tail.after = NULL;
}
void InsertNode(BULLET bullet)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->bullet = bullet;
	newNode->before = &head;
	newNode->after = head.after;
	head.after->before = newNode;
	head.after = newNode;
}
void DeleteNode(NODE* deleteNode)
{
	deleteNode->before->after = deleteNode->after;
	deleteNode->after->before = deleteNode->before;
	free(deleteNode);
}
void DeleteAllNode(void)
{
	NODE* findNode = head.after;
	NODE* deleteNode = NULL;
	while (findNode != &tail)
	{
		deleteNode = findNode;
		findNode = findNode->after;

		free(deleteNode);
	}
	InitHeadTail();
}

void SetTextColor(int bgColor, int textColor)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, (bgColor << 4) + textColor);
}

void GotoxyDraw(int x, int y, const char* str)
{
	COORD pos = { (SHORT)(x * 2), (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", str);
}
void GotoxyDrawC(int x, int y, const char c)
{
	COORD pos = { (SHORT)(x * 2), (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%c", c);
}
void Gotoxy(int x, int y)
{
	COORD pos = { (SHORT)(x * 2), (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void SetNoCursor(void)
{
	CONSOLE_CURSOR_INFO CurInfo = { 0 };
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void Title(void)
{
	SetTextColor(COLOR_BLACK, COLOR_BLACK);
	system("cls");
	int nCnt = 0;
	SetTextColor(COLOR_BLACK, COLOR_WHITE);
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 2, "■■■■■■■■■■■■■■■■");
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 3, "■                            ■");
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 4, "■                            ■");
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 5, "■                            ■");
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 6, "■■■■■■■■■■■■■■■■");
	Sleep(50);
	SetTextColor(COLOR_BLACK, COLOR_GREEN);
	GotoxyDraw(OFFSET_TITLE_X + 16, OFFSET_TITLE_Y + 4, " 탄");
	Sleep(50);
	GotoxyDraw(OFFSET_TITLE_X + 16, OFFSET_TITLE_Y + 4, " 탄 막");
	Sleep(50);
	GotoxyDraw(OFFSET_TITLE_X + 16, OFFSET_TITLE_Y + 4, " 탄 막 피");
	Sleep(50);
	GotoxyDraw(OFFSET_TITLE_X + 16, OFFSET_TITLE_Y + 4, " 탄 막 피 하");
	Sleep(50);
	GotoxyDraw(OFFSET_TITLE_X + 16, OFFSET_TITLE_Y + 4, " 탄 막 피 하 기");
	Sleep(50);
	SetTextColor(COLOR_BLACK, COLOR_YELLOW);
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 11, "           △ ");
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 12, "         ◁  ▷ : 이동");
	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 13, "           ▽");
	GotoxyDraw(OFFSET_TITLE_X + 9, OFFSET_TITLE_Y + 14, "                SPACE : 일시정지");
	GotoxyDraw(OFFSET_TITLE_X + 9, OFFSET_TITLE_Y + 15, "  (메인 화면에서) ESC : Exit");

	while (_kbhit())
	{
		_getch();//버퍼에 기록된 키값을 버림 
	}

	GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 9, "★ Please Enter Any Key to Start ★");
	while (TRUE)
	{
		if (_kbhit())
		{
			if (_getch() == KEY_ESC)
			{
				Exit();
			}
			break;
		}
		if ((nCnt % 25) == 0)
		{
			GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 9, "                                                                 ");
		}
		if (nCnt % 50 == 0)
		{
			GotoxyDraw(OFFSET_TITLE_X + 12, OFFSET_TITLE_Y + 9, "★ Please Enter Any Key to Start ★");
			nCnt = 0;
		}

		Sleep(10); // 00.1초 딜레이  
		nCnt++;
	}

	while (_kbhit())
	{
		_getch();//버퍼에 기록된 키값을 버림 
	}
}
void Game(void)
{
	system("cls");
	myPos.X = OFFSET_GAME_X + DEFAULTPOS_X + 1;
	myPos.Y = OFFSET_GAME_Y + DEFAULTPOS_Y + 1;
	bulletCreateSpd = DEFAULT_BULLET_CREATE_SPD;
	bulletMoveSpd = DEFAULT_BULLET_MOVE_SPD;
	hp = HP_MAX;
	score = 0;
	int levelCnt = LEVEL_CNT;
	int scoreCnt = SCORE_CNT;
	DrawWall();
	UpdateHpUI();
	UpdateSubUI();
	UpdateScoreUI();
	float bulletCreateCnt = bulletCreateSpd;
	SetTextColor(GAME_BG_COLOR, PLAYER_COLOR);
	GotoxyDraw(myPos.X, myPos.Y, DRAW_PLAYER);
	while (TRUE)
	{
		KeyControl(); // 키입력

		if (bulletCreateCnt <= 0) //총알 생성
		{
			CreateBullet();
			bulletCreateCnt = bulletCreateSpd;
		}	
		if (!BulletProcessing()) //총알 데이터 처리 / 게임 오버시 FALSE 리턴.
		{
			DeleteAllNode();
			while (_kbhit())
			{
				_getch();//버퍼에 기록된 키값을 버림 
			}
			ShowGameEnd();
			_getch();//대기(임시)
			return;
		}

		SetTextColor(GAME_BG_COLOR, PLAYER_COLOR); //플레이어를 맨 앞에 그림
		GotoxyDraw(myPos.X, myPos.Y, DRAW_PLAYER); 

		if (levelCnt <= 0)
		{	
			bulletCreateSpd -= bulletCreateSpd * ((float)BULLET_CREATE_SPD_UNIT / 100.0f);
			bulletMoveSpd -= bulletMoveSpd * ((float)BULLET_MOVE_SPD_UNIT / 100.0f);		
			UpdateSubUI();
			levelCnt = LEVEL_CNT;
		}
		if (scoreCnt <= 0)
		{
			score += SCORE_UNIT;
			UpdateScoreUI();
			scoreCnt = SCORE_CNT;
		}


		levelCnt -= UPDATE_UNIT;
		scoreCnt -= UPDATE_UNIT;
		bulletCreateCnt -= UPDATE_UNIT;
		Sleep(UPDATE_UNIT);
	}
}
void Exit(void)
{
	system("cls");
	SetTextColor(COLOR_BLACK, COLOR_WHITE);

	for (int i = UI_POS_END_X; i < UI_POS_END_X + UI_SIZE_END_X; i++)
	{
		for (int j = UI_POS_END_Y; j < UI_POS_END_Y + UI_SIZE_END_Y; j++)
		{
			if ((i == UI_POS_END_X) || (i == UI_POS_END_X + UI_SIZE_END_X - 1) ||
				(j == UI_POS_END_Y) || (j == UI_POS_END_Y + UI_SIZE_END_Y - 1))
			{
				GotoxyDraw(i, j, DRAW_UI_END_FRAME);
			}
			else
			{
				GotoxyDraw(i, j, DRAW_EMPTY);
			}
		}
	}
	GotoxyDraw(UI_POS_END_X + 8, UI_POS_END_Y + 7, "게임 프로그램을 종료합니다.");
	Gotoxy(0, UI_POS_END_Y + UI_SIZE_END_Y + 1);
	exit(0);
}

void Init(void)
{
	char strWinSize[30] = { 0 };
	sprintf_s(strWinSize, 30, "mode con:cols=%d lines=%d", 
		(OFFSET_GAME_X + SIZE_GAMEMAP + 10) * 2 + 15,
		OFFSET_GAME_Y + SIZE_GAMEMAP + 10);
	system(strWinSize);
	srand((unsigned int)time(NULL));
	SetNoCursor();
	InitHeadTail();
	for (int i = 0; i < SIZE_WALL; i++)
	{
		arrWall[WALL_LEFT][i].X = OFFSET_GAME_X;
		arrWall[WALL_LEFT][i].Y = OFFSET_GAME_Y + i + 1;
	}
	for (int i = 0; i < SIZE_WALL; i++)
	{
		arrWall[WALL_RIGHT][i].X = OFFSET_GAME_X + SIZE_GAMEMAP;
		arrWall[WALL_RIGHT][i].Y = OFFSET_GAME_Y + i + 1;
	}
	for (int i = 0; i < SIZE_WALL; i++)
	{
		arrWall[WALL_UP][i].X = OFFSET_GAME_X + i + 1;
		arrWall[WALL_UP][i].Y = OFFSET_GAME_Y;
	}
	for (int i = 0; i < SIZE_WALL; i++)
	{
		arrWall[WALL_DOWN][i].X = OFFSET_GAME_X + i + 1;
		arrWall[WALL_DOWN][i].Y = OFFSET_GAME_Y + SIZE_GAMEMAP;
	}

	sprintf_s(arrBulletDir[BULLET_NULL], 3, "\0");
	sprintf_s(arrBulletDir[BULLET_UP], 3, "↑");
	sprintf_s(arrBulletDir[BULLET_UP_R], 3, "↗");
	sprintf_s(arrBulletDir[BULLET_RIGHT], 3, "→");
	sprintf_s(arrBulletDir[BULLET_DOWN_R], 3, "↘");
	sprintf_s(arrBulletDir[BULLET_DOWN], 3, "↓");
	sprintf_s(arrBulletDir[BULLET_DOWN_L], 3, "↙");
	sprintf_s(arrBulletDir[BULLET_LEFT], 3, "←");
	sprintf_s(arrBulletDir[BULLET_UP_L], 3, "↖");
	
}

void DrawWall(void)
{	
	SetTextColor(WALL_COLOR, WALL_COLOR);
	for (int i = 0; i < WALL_DIR_MAX; i++)
	{
		for (int j = 0; j < SIZE_WALL; j++)
		{
			GotoxyDraw(arrWall[i][j].X, arrWall[i][j].Y, DRAW_GAME_WALL);
		}
	}
}

void CreateBullet(void)
{
	int createWallDir = 0;
	int createRandomPos = 0;
	NODE* findNode = NULL;
	BOOL isOverLap = FALSE;
	do
	{
		createWallDir = rand() % WALL_DIR_MAX;
		createRandomPos = rand() % SIZE_WALL;

		isOverLap = FALSE;
		findNode = head.after;
		while (findNode != &tail)
		{
			if ((findNode->bullet.wCnt > 0) &&
				((findNode->bullet.pos.X == arrWall[createWallDir][createRandomPos].X) &&
					(findNode->bullet.pos.Y == arrWall[createWallDir][createRandomPos].Y)))
			{
				isOverLap = TRUE;
			}
			findNode = findNode->after;
		}

	} while (isOverLap);

	BULLET newBullet = { 0 };
	newBullet.isActive = FALSE;
	newBullet.fMoveTime = bulletMoveSpd;
	newBullet.wCnt = BULLET_W_CNT;
	newBullet.pos = arrWall[createWallDir][createRandomPos];
	newBullet.trace = { 0 };
	switch (createWallDir)
	{
	case WALL_LEFT:
	{
		if (createRandomPos == 0) // 왼쪽 맨 위일 경우엔 위+오른쪽 방향 불가능
		{
			newBullet.Dir = 3 + rand() % 2;	//3,4
		}
		else if (createRandomPos == SIZE_WALL - 1) //왼쪽 맨 아래일 경우엔 아래+오른쪽 방향 불가능
		{
			newBullet.Dir = 2 + rand() % 2;	//2,3
		}
		else
		{
			newBullet.Dir = 2 + (rand() % 3);	//2,3,4
		}
	}
	break;
	case WALL_RIGHT:
	{
		if (createRandomPos == 0) //오른쪽 맨 위일 경우엔 위+왼쪽 방향 불가능
		{
			newBullet.Dir = 6 + (rand() % 2);	//6,7
		}
		else if (createRandomPos == SIZE_WALL - 1) //오른쪽 맨 아래일 경우엔 아래+왼쪽 방향 불가능
		{
			newBullet.Dir = 7 + (rand() % 2);	//7,8
		}
		else
		{
			newBullet.Dir = 6 + (rand() % 3);	//6,7,8
		}
	}
	break;
	case WALL_UP:
	{
		if (createRandomPos == 0) //위쪽 맨 왼쪽일 경우 아래+왼쪽 방향 불가능
		{
			newBullet.Dir = 4 + (rand() % 2);	//4,5
		}
		else if (createRandomPos == SIZE_WALL - 1) //위쪽 맨 오른쪽일 경우 아래+오른쪽 방향 불가능
		{
			newBullet.Dir = 5 + (rand() % 2);	//5,6
		}
		else
		{
			newBullet.Dir = 4 + (rand() % 3);	//4,5,6
		}
	}
	break;
	case WALL_DOWN:
	{
		if (createRandomPos == 0) //아래쪽 맨 왼쪽일 경우 위+왼쪽 방향 불가능
		{
			newBullet.Dir = 1 + (rand() % 2);	//1,2
		}
		else if (createRandomPos == SIZE_WALL - 1) //아래쪽 맨 오른쪽일 경우 위+오른쪽 방향 불가능
		{
			newBullet.Dir = (7 + (rand() % 2)) % 8 + 1;	//1,8
		}
		else
		{
			newBullet.Dir = (7 + (rand() % 3)) % 8 + 1;	//8,1,2
		}
	}
	break;
	}

	InsertNode(newBullet);
}

void MoveBulletData(BULLET* pBullet)
{
	switch (pBullet->Dir)
	{
	case BULLET_UP:
	{
		pBullet->pos.Y--;
	}
	break;
	case BULLET_UP_R:
	{
		pBullet->pos.X++;
		pBullet->pos.Y--;		
	}
	break;
	case BULLET_RIGHT:
	{
		pBullet->pos.X++;
	}
	break;
	case BULLET_DOWN_R:
	{
		pBullet->pos.X++;
		pBullet->pos.Y++;
	}
	break;
	case BULLET_DOWN:
	{
		pBullet->pos.Y++;
	}
	break;
	case BULLET_DOWN_L:
	{
		pBullet->pos.X--;
		pBullet->pos.Y++;
	}
	break;
	case BULLET_LEFT:
	{
		pBullet->pos.X--;
	}
	break;
	case BULLET_UP_L:
	{
		pBullet->pos.X--;
		pBullet->pos.Y--;
	}
	break;
	}
}

void KeyControl(void)
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (!CheckColWall(myPos.X - 1, myPos.Y))
		{
			SetTextColor(GAME_BG_COLOR, GAME_BG_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_EMPTY);
			myPos.X--;
			SetTextColor(GAME_BG_COLOR, PLAYER_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_PLAYER);
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (!CheckColWall(myPos.X + 1, myPos.Y))
		{
			SetTextColor(GAME_BG_COLOR, GAME_BG_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_EMPTY);
			myPos.X++;
			SetTextColor(GAME_BG_COLOR, PLAYER_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_PLAYER);
		}
	}
	if (GetAsyncKeyState(VK_UP))
	{
		if (!CheckColWall(myPos.X, myPos.Y - 1))
		{
			SetTextColor(GAME_BG_COLOR, GAME_BG_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_EMPTY);
			myPos.Y--;
			SetTextColor(GAME_BG_COLOR, PLAYER_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_PLAYER);
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (!CheckColWall(myPos.X, myPos.Y + 1))
		{
			SetTextColor(GAME_BG_COLOR, GAME_BG_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_EMPTY);
			myPos.Y++;
			SetTextColor(GAME_BG_COLOR, PLAYER_COLOR);
			GotoxyDraw(myPos.X, myPos.Y, DRAW_PLAYER);
		}
	}

	if (_kbhit())
	{
		if (_getch() == KEY_SPACE)
		{
			GotoxyDraw(20, 0, "일시정지 중");
			while (TRUE)
			{
				if (_getch() == KEY_SPACE)
				{
					break;
				}
				Sleep(UPDATE_UNIT);
			}
			GotoxyDraw(20, 0, "                                 ");
		}
	}
}

void UpdateHpUI(void)
{
	SetTextColor(HP_BG_COLOR, HP_COLOR);
	Gotoxy(UI_POS_HP_X, UI_POS_HP_Y);

	for (int i = 0; i < hp; i++)
	{
		printf("%s", DRAW_HP);
	}
	for (int i = 0; i < HP_MAX - hp; i++)
	{
		printf("%s", DRAW_EMPTY);
	}
}

BOOL BulletProcessing(void)
{
	NODE* findNode = head.after;
	while (findNode != &tail) //모든 총알 처리
	{
		if (findNode->bullet.wCnt > 0) //위험 카운팅 중일 경우
		{
			SetTextColor(WARNING_COLOR, GAME_BG_COLOR);
			GotoxyDraw(findNode->bullet.pos.X, findNode->bullet.pos.Y, arrBulletDir[findNode->bullet.Dir]);
			findNode->bullet.wCnt -= UPDATE_UNIT;
		}
		else if (!findNode->bullet.isActive) //엑티브 상태로 전환하는 경우(처음 한번 이동은 절대 벽과 충돌하지 않으며, 초기 위치 세팅을 위한 이동이다.)
		{
			findNode->bullet.isActive = TRUE;
			SetTextColor(WALL_COLOR, WALL_COLOR);
			GotoxyDraw(findNode->bullet.pos.X, findNode->bullet.pos.Y, DRAW_GAME_WALL);
			MoveBulletData(&(findNode->bullet));
			SetTextColor(GAME_BG_COLOR, BULLET_COLOR);
			GotoxyDraw(findNode->bullet.pos.X, findNode->bullet.pos.Y, DRAW_BULLET);
		}
		else //이동시키는 경우
		{
			if (findNode->bullet.fMoveTime <= 0)
			{
				SetTextColor(GAME_BG_COLOR, GAME_BG_COLOR);
				GotoxyDraw(findNode->bullet.pos.X, findNode->bullet.pos.Y, DRAW_EMPTY);
				if (findNode->bullet.trace.X != 0 && findNode->bullet.trace.Y != 0)
				{
					GotoxyDraw(findNode->bullet.trace.X, findNode->bullet.trace.Y, DRAW_EMPTY);
				}
				findNode->bullet.trace.X = findNode->bullet.pos.X;
				findNode->bullet.trace.Y = findNode->bullet.pos.Y;
				MoveBulletData(&(findNode->bullet));
				if (findNode->bullet.pos.X <= OFFSET_GAME_X ||
					findNode->bullet.pos.X >= OFFSET_GAME_X + SIZE_GAMEMAP ||
					findNode->bullet.pos.Y <= OFFSET_GAME_Y ||
					findNode->bullet.pos.Y >= OFFSET_GAME_Y + SIZE_GAMEMAP)
				{
					findNode = findNode->after;
					DeleteNode(findNode->before);
					continue;
				}
				else
				{
					SetTextColor(GAME_BG_COLOR, BULLET_COLOR);
					GotoxyDraw(findNode->bullet.pos.X, findNode->bullet.pos.Y, DRAW_BULLET);
					GotoxyDraw(findNode->bullet.trace.X, findNode->bullet.trace.Y, arrBulletDir[findNode->bullet.Dir]);
					findNode->bullet.fMoveTime = bulletMoveSpd;
				}
			}
			else
			{
				findNode->bullet.fMoveTime -= UPDATE_UNIT;
			}
		}
		if (findNode->bullet.pos.X == myPos.X && //플레이어와 충돌
			findNode->bullet.pos.Y == myPos.Y)
		{
			if (findNode->bullet.trace.X != 0 && findNode->bullet.trace.Y != 0)
			{
				SetTextColor(GAME_BG_COLOR, GAME_BG_COLOR);
				GotoxyDraw(findNode->bullet.trace.X, findNode->bullet.trace.Y, DRAW_EMPTY);
			}
			hp--;
			UpdateHpUI();
			findNode = findNode->after;
			DeleteNode(findNode->before);
			if (hp <= 0) //게임오버
			{
				return FALSE;
			}
			continue;
		}
		findNode = findNode->after;
	}
	return TRUE;
}

void UpdateSubUI(void)
{
	SetTextColor(GAME_BG_COLOR, COLOR_WHITE);

	Gotoxy(OFFSET_GAME_X + SIZE_GAMEMAP + 3, 3);
	printf("bulletCreateSpd : %.2f      ", bulletCreateSpd);
	Gotoxy(OFFSET_GAME_X + SIZE_GAMEMAP + 3, 4);
	printf("bulletMoveSpd : %.2f       ", bulletMoveSpd);
}

void UpdateScoreUI(void)
{
	SetTextColor(GAME_BG_COLOR, COLOR_YELLOW);

	Gotoxy(OFFSET_GAME_X + SIZE_GAMEMAP + 3, 5);
	printf("Score : %d         ", score);
}

void ShowGameEnd(void)
{
	SetTextColor(GAME_BG_COLOR, COLOR_WHITE);

	for (int i = UI_END_POS_X; i < UI_END_POS_X + UI_END_SIZE_X; i++)
	{
		for (int j = UI_END_POS_Y; j < UI_END_POS_Y + UI_END_SIZE_Y; j++)
		{
			if (i == UI_END_POS_X ||
				i == UI_END_POS_X + UI_END_SIZE_X - 1 ||
				j == UI_END_POS_Y ||
				j == UI_END_POS_Y + UI_END_SIZE_Y - 1)
			{
				GotoxyDraw(i, j, "▣");
			}
			else
			{
				GotoxyDraw(i, j, DRAW_EMPTY);
			}
		}
	}
	Gotoxy(UI_END_POS_X + 8, UI_END_POS_Y + 4);
	printf("점수 : %d", score);
	Gotoxy(UI_END_POS_X + 2, UI_END_POS_Y + 6);
	printf("아무 키나 누르면 메인으로 돌아갑니다");

}

BOOL CheckColWall(int x, int y)
{
	if (x <= OFFSET_GAME_X ||
		x >= OFFSET_GAME_X + SIZE_GAMEMAP ||
		y <= OFFSET_GAME_Y ||
		y >= OFFSET_GAME_Y + SIZE_GAMEMAP)
	{
		return TRUE;
	}
	return FALSE;
}

