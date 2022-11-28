#pragma once
class Chess {
private:
	int** chessboard;
	int size, blackid, whiteid;
	long long begintime, endtime;
public:
	Chess(int, int, int);
	void LoadChess(){}
	void SetChess(){}
	void CheckEnd(){}
	void Simulate(){}
	void LoadBoard(){}
	void SaveBoard(){}
	void End(){}

	int Size() { return size; }
	int Blackid() { return blackid; }
	int Whiteid() { return whiteid; }
	long long Time() { return begintime; }
};