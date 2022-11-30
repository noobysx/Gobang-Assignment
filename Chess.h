#pragma once

#include "framework.h"
#define PIECE_BLACK		1
#define PIECE_WHITE		2
#define PIECE_HOVER		4
#define PIECE_SET		8
#define STATUS_GAMING	1
#define STATUS_SUSPEND	2
#define STATUS_BLACKWIN	3
#define STATUS_WHITEWIN 4
#define STATUS_DRAW		5
#define STATUS_ILLEGAL	6
#define FILE_MOD 1000000007
#define BUFFER_MAXSIZE 1000

typedef char BOARDUNIT;

class Chess {
private:
	BOARDUNIT** chessboard;
	int size, blackid, whiteid, currentcolor, status;//对局参数 【存储】
	long long begintime, endtime, timelimit;//【存储】
	int clientborder, boardlength, leftborder, gridborder, chessradius;//界面参数
	double gridlength;
	POINT vertex[4];
	POINT hover[2];//鼠标悬停
	COLORREF boardcolor;//背景颜色 【存储】
	std::vector<POINT> step;//存储步数 【存储】
public:
	Chess(int, int, int);//初始化
	void CreateChessboard();
	void MouseOperation(int, int, int);//鼠标悬停及点击操作
	void SetChess(int, int, int);//放置一枚棋子
	void CheckEnd();//检查是否结束
	void CheckEndLatest();//检查最后一颗棋子是否造成一方胜利
	void Simulate(){}
	void LoadBoardAsBinary(const char*);//载入棋局
	void SaveBoardAsBinary(const char*);//保存棋局
	void BinaryToJson(const char*){}
	void JsonToBinary(const char*){}
	void End();
	void Suspend();
	void PaintBoard(HDC, RECT*);//画棋盘
	void DrawChess(HDC, int, COLORREF);//画所有指定颜色棋子
	void DrawAChess(HDC, int, int);//画一个棋子
	void PaintHover(HDC);//绘制鼠标悬停棋子显示

	int Size() { return size; }
	int Blackid() { return blackid; }
	int Whiteid() { return whiteid; }
	long long Begintime() { return begintime; }
};