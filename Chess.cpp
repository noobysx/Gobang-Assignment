#include "Chess.h"
#include <time.h>

Chess::Chess(int s, int b, int w) :size(s), blackid(b), whiteid(w) {
	chessboard = new int* [size];
	for (int i = 0; i < size; ++i) {
		chessboard[i] = new int[size];
	}
	begintime = time(nullptr);
	SaveBoard();
}

