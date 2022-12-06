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

class Gobang {
protected:
	BOARDUNIT** chessboard;
	int size;
};

class Chess: public Gobang {
private:
	int blackid, whiteid, currentcolor, status;//�Ծֲ��� ���洢��
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

#define PATTERNMAX 4
class Gamenode {
public:
	std::vector<Gamenode*> next;
	Gamenode* father;
	char point[2], color;
	int score;
};

class Trie {
public:
	Trie* next[3], * fail;
	char exist;
	Trie();
	void insert(const char*, int, int);
};

class AI : public Gobang {
private:
	Gamenode* gametree;
	Trie* match;
	const char* patterns[PATTERNMAX] = { "011","111","211","012" };
	const int* weights[PATTERNMAX] = { 0,0,0,0 };
public:
	AI();
	int Estimate(){}//�Ե�ǰ�����ֵ�����С��С�б��Query
	void Calculate() {}//������һ��
	void Query(const char*, Trie*, int*);//����AC�Զ�������ĳ�����ӵ�ƥ������

	//��Chess��Ľӿ�
};