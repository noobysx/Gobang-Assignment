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
	int size, blackid, whiteid, currentcolor, status;//�Ծֲ��� ���洢��
	long long begintime, endtime, timelimit;//���洢��
	int clientborder, boardlength, leftborder, gridborder, chessradius;//�������
	double gridlength;
	POINT vertex[4];
	POINT hover[2];//�����ͣ
	COLORREF boardcolor;//������ɫ ���洢��
	std::vector<POINT> step;//�洢���� ���洢��
public:
	Chess(int, int, int);//��ʼ��
	void CreateChessboard();
	void MouseOperation(int, int, int);//�����ͣ���������
	void SetChess(int, int, int);//����һö����
	void CheckEnd();//����Ƿ����
	void CheckEndLatest();//������һ�������Ƿ����һ��ʤ��
	void Simulate(){}
	void LoadBoardAsBinary(const char*);//�������
	void SaveBoardAsBinary(const char*);//�������
	void BinaryToJson(const char*){}
	void JsonToBinary(const char*){}
	void End();
	void Suspend();
	void PaintBoard(HDC, RECT*);//������
	void DrawChess(HDC, int, COLORREF);//������ָ����ɫ����
	void DrawAChess(HDC, int, int);//��һ������
	void PaintHover(HDC);//���������ͣ������ʾ

	int Size() { return size; }
	int Blackid() { return blackid; }
	int Whiteid() { return whiteid; }
	long long Begintime() { return begintime; }
};