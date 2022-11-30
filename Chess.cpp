#include "Chess.h"
#include "framework.h"

Chess::Chess(int s, int b, int w) :size(s), blackid(b), whiteid(w) {
    CreateChessboard();
    currentcolor = PIECE_BLACK;
    boardcolor = RGB(255, 227, 132);
    timelimit = -1;
	begintime = time(nullptr);
	//SaveBoard();
}

inline void Chess::DrawAChess(HDC hdc, int x, int y){
    Ellipse(hdc, x - chessradius, y - chessradius, x + chessradius, y + chessradius);
}

void Chess::DrawChess(HDC hdc, int flag, COLORREF color) {
    HBRUSH brh = CreateSolidBrush(color);
    SelectObject(hdc, brh);
    chessradius = boardlength / size / 2 - 1;
    for (int i = 0; i < size; ++i) {
        int rx = vertex[0].x + gridborder + i * gridlength;
        for (int j = 0; j < size; ++j) {
            if (chessboard[i][j] == flag) {
                DrawAChess(hdc, rx, vertex[0].y + gridborder + j * gridlength);
            }
        }
    }
    DeleteObject(brh);
}

void Chess::PaintHover(HDC hdc) {
    if (hover[1].x >= 0 && hover[1].x < size && hover[1].y >= 0 && hover[1].y < size) {
        HBRUSH brh;
        if (currentcolor == PIECE_BLACK) brh = CreateSolidBrush(RGB(115, 115, 115));
        else brh = CreateSolidBrush(RGB(225, 225, 225));
        SelectObject(hdc, brh);
        DrawAChess(hdc, vertex[0].x + gridborder + hover[1].x * gridlength, vertex[0].y + gridborder + hover[1].y * gridlength);
        DeleteObject(brh);
    }
    if (hover[0].x >= 0 && hover[0].x < size && hover[0].y >= 0 && hover[0].y < size) {
        HBRUSH brh = CreateSolidBrush(boardcolor);
        int cx = vertex[0].x + gridborder + hover[0].x * gridlength;
        int cy = vertex[0].y + gridborder + hover[0].y * gridlength;
        RECT outline;
        outline.left = cx - chessradius;
        outline.right = cx + chessradius;
        outline.top = cy - chessradius;
        outline.bottom = cy + chessradius;
        FillRect(hdc, &outline, brh);
        DeleteObject(brh);
        MoveToEx(hdc, max(outline.left ,vertex[0].x + gridborder), cy, nullptr);
        LineTo(hdc, min(outline.right, vertex[1].x - gridborder), cy);
        MoveToEx(hdc, cx, max(outline.top, vertex[0].y + gridborder), nullptr);
        LineTo(hdc, cx, min(outline.bottom, vertex[2].y - gridborder));
    }
}

void Chess::PaintBoard(HDC hdc, RECT* wndr) {
    HBRUSH brh = CreateSolidBrush(boardcolor);
    SelectObject(hdc, brh);
    clientborder = (wndr->bottom - wndr->top) / 100;
    boardlength = wndr->bottom - wndr->top - clientborder * 2;
    leftborder = (wndr->right - wndr->left - boardlength) >> 1;
    vertex[0].y = clientborder, vertex[0].x = leftborder;
    vertex[1].y = clientborder, vertex[1].x = leftborder + boardlength;
    vertex[2].y = boardlength + clientborder, vertex[2].x = leftborder + boardlength;
    vertex[3].y = boardlength + clientborder, vertex[3].x = leftborder;
    Polygon(hdc, vertex, 4);
    gridborder = boardlength / size / 2;
    gridlength = 1.0 * (boardlength - gridborder * 2) / (size - 1);
    for (int i = 0; i < size; ++i) {
        MoveToEx(hdc, vertex[0].x + gridborder + i * gridlength, vertex[0].y + gridborder, NULL);
        LineTo(hdc, vertex[3].x + gridborder + i * gridlength, vertex[3].y - gridborder);
        MoveToEx(hdc, vertex[0].x + gridborder, vertex[0].y + gridborder + i * gridlength, NULL);
        LineTo(hdc, vertex[1].x - gridborder, vertex[1].y + gridborder + i * gridlength);
    }
    DeleteObject(brh);
    DrawChess(hdc, PIECE_BLACK, RGB(0, 0, 0));
    DrawChess(hdc, PIECE_WHITE, RGB(255, 255, 255));
}

void Chess::MouseOperation(int x, int y, int flag) {
    if (status != STATUS_GAMING) return;
    x -= leftborder;
    y -= clientborder;
    int posx = -1, posy = -1;
    if (x > 0 && x < boardlength && y > 0 && y < boardlength) {
        posx =  x / gridlength;
        posy =  y / gridlength;
        if (flag == PIECE_SET) SetChess(posx, posy, flag);
        else if (flag == PIECE_HOVER) {
            if ((posx != hover[1].x || posy != hover[1].y) && chessboard[posx][posy] == 0) {
                hover[0] = hover[1];
                hover[1] = { posx, posy };
            }
        }
    }
}

void Chess::CheckEnd() {
    int wwin = 0, bwin = 0;
    auto win = [](auto& a, auto& b, auto& c) {a = 0; b++; if (b == 5) b = 0, c++; };
    auto func = [&](const auto def1, const auto def2) {
        int w1, w2, b1, b2;
        for (int i = 0; i < size; ++i) {
            w1 = 0, w2 = 0, b1 = 0, b2 = 0;
            for (int j = 0; j < size; ++j) {
                if (chessboard[i][j] == PIECE_BLACK || chessboard[i][j] == def1) win(w1, b1, bwin);
                else if (chessboard[i][j] == PIECE_WHITE || chessboard[i][j] == def2) win(b1, w1, wwin);
                else w1 = b1 = 0;
                if (chessboard[j][i] == PIECE_BLACK || chessboard[j][i] == def1) win(w2, b2, bwin);
                else if (chessboard[j][i] == PIECE_WHITE || chessboard[j][i] == def2) win(b2, w2, wwin);
                else w2 = b2 = 0;
            }
        }
        for (int i = 4; i < size; ++i) {
            w1 = 0, w2 = 0, b1 = 0, b2 = 0;
            for (int j = i; j >= 0; --j) {
                if (chessboard[j][i - j] == PIECE_BLACK || chessboard[j][i - j] == def1) win(w1, b1, bwin);
                else if (chessboard[j][i - j] == PIECE_WHITE || chessboard[j][i - j] == def2) win(b1, w1, wwin);
                else w1 = b1 = 0;
                if (chessboard[i - j][size - j - 1] == PIECE_BLACK || chessboard[i - j][size - j - 1] == def1) win(w2, b2, bwin);
                else if (chessboard[i - j][size - j - 1] == PIECE_WHITE || chessboard[i - j][size - j - 1] == def2) win(b2, w2, wwin);
                else w2 = b2 = 0;
            }
        }
        for (int i = 1; i < size - 4; ++i) {
            w1 = 0, w2 = 0, b1 = 0, b2 = 0;
            for (int j = size - 1; j >= i; --j) {
                if (chessboard[j][size - j + i - 1] == PIECE_BLACK || chessboard[j][size - j + i - 1] == def1) win(w1, b1, bwin);
                else if (chessboard[j][size - j + i - 1] == PIECE_WHITE || chessboard[j][size - j + i - 1] == def2) win(b1, w1, wwin);
                else w1 = b1 = 0;
                if (chessboard[j][j - i] == PIECE_BLACK || chessboard[j][j - i] == def1) win(w2, b2, bwin);
                else if (chessboard[j][j - i] == PIECE_WHITE || chessboard[j][j - i] == def2) win(b2, w2, wwin);
                else w2 = b2 = 0;
            }
        }
    };
    func(-1, -1);
    if (wwin != 0 && bwin != 0) status = STATUS_ILLEGAL;
    else if (wwin == 0 && bwin != 0) status = STATUS_BLACKWIN;
    else if (wwin != 0 && bwin == 0) status = STATUS_WHITEWIN;
    else {
        func(0, -1);
        func(-1, 0);
        if (wwin == 0 && bwin == 0) status = STATUS_DRAW;
    }
}

void Chess::CheckEndLatest() {
    if (step.size() == 0) return;
    int x = step.back().x, y = step.back().y, color = currentcolor == PIECE_BLACK ? PIECE_WHITE : PIECE_BLACK;
    const POINT d[8] = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,-1},{1,-1},{-1,1} };
    int cnt;
    for (int i = 0; i < 8; ++i) {
        if ((i & 1) == 0) cnt = 1;
        int nx = x, ny = y;
        for (int j = 1; j < 5; ++j) {
            nx += d[i].x, ny += d[i].y;
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && chessboard[nx][ny] == color) cnt++;
            else break;
        }
        if (cnt >= 5) {
            status = color == PIECE_BLACK ? STATUS_BLACKWIN : STATUS_WHITEWIN;
            return;
        }
    }
}

void Chess::SetChess(int x, int y, int flag) {
    if (chessboard[x][y] != 0 || status != STATUS_GAMING) return;
    flag |= currentcolor;
    if (flag & PIECE_BLACK) {
        chessboard[x][y] = PIECE_BLACK;
        currentcolor = PIECE_WHITE;
    }else if (flag & PIECE_WHITE) {
        chessboard[x][y] = PIECE_WHITE;
        currentcolor = PIECE_BLACK;
    }
    if (hover[1].x == x && hover[1].y == y) hover[1] = { -1,-1 };
    step.push_back({ x,y });
    CheckEndLatest();
    //SaveBoard("file.dat");
}

void Chess::SaveBoardAsBinary(const char* filename) {
    std::ofstream outfile;
    outfile.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    char buffer[BUFFER_MAXSIZE];
    int pos = 0;
    memset(buffer, 0, sizeof(buffer));
    auto ib = [&](const auto& var) {//In Buffer
        auto tmp = var;
        for (int i0 = 0; i0 < sizeof(var); ++i0, tmp>>=8) buffer[pos + i0] = tmp;
        pos += sizeof(var);
    };
    ib((char)size); ib((char)blackid); ib((char)whiteid); ib((char)currentcolor); ib((char)status);
    ib(begintime); ib(endtime); ib(timelimit); ib(boardcolor);
    ib((int)step.size());//到此位置37
    for (const auto& sp : step) {
        ib((char)sp.x); ib((char)sp.y);
    }
    int code = 0;
    for (int i = 0; i < pos; ++i) code = (code + (unsigned char)buffer[i])% FILE_MOD;//前面所有字节的加和
    ib(code);//末尾校验码
    outfile.write((const char*)buffer, pos);
    outfile.close();
}

void Chess::LoadBoardAsBinary(const char* filename) {
    End();
    std::ifstream infile;
    infile.open(filename, std::ios::binary);
    char buffer[BUFFER_MAXSIZE];
    int pos = 0;
    infile.read(buffer, 37);
    auto ob = [&](auto& var, int size) {
        var = 0;
        for (int i0 = size-1; i0 >=0; --i0) {
            var <<= 8;
            var += (unsigned char)buffer[pos + i0];
        }
        pos += size;
    };
    ob(size, 1); ob(blackid, 1); ob(whiteid, 1); ob(currentcolor, 1); ob(status, 1);
    ob(begintime, 8); ob(endtime, 8); ob(timelimit, 8); 
    ob(boardcolor, 4);
    int vs; ob(vs, 4); step.resize(vs);
    infile.read(&buffer[37], (vs << 1) + 4);
    for (int i = 0; i < vs; ++i) {
        ob(step[i].x, 1); ob(step[i].y, 1);
    }
    int code = 0, codeori;
    for (int i = 0; i < pos; ++i) code = (code + (unsigned char)buffer[i]) % FILE_MOD;
    ob(codeori, 4);
    if (code != codeori) {
        MessageBox(0, L"code error", L"fail", 0);
        End();
        return;
    }
    CreateChessboard();//载入参数完成，重新开始
    for (int i = 0; i < vs; ++i) SetChess(step[i].x, step[i].y, PIECE_SET);
    CheckEnd();
}

void Chess::CreateChessboard() {
    chessboard = new BOARDUNIT * [size];
    for (int i = 0; i < size; ++i) chessboard[i] = new BOARDUNIT[size]();
    hover[0] = hover[1] = { -1,-1 };
    status = STATUS_GAMING;
}

void Chess::End() {
    if (status == STATUS_GAMING) status = STATUS_SUSPEND;
    for (int i = 0; i < size; ++i) delete chessboard[i];
    delete chessboard;
}

void Chess::Suspend() {
    status = STATUS_SUSPEND;
}