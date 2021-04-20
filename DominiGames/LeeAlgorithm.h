#pragma once

#include <vector>

#include <iostream>
#include <iomanip>

#include "Tile.h"

struct Pos
{
    int x, y;
};

class LeeAlgorithm
{
    static const int BLANK = 0;
    static const int START = 2;
    static const int FINISH = 3;
    static const int WAVE_START = 4;
    static const int M = 8;

    int arr[M][M]; // work field
    int len;
    int px[M * M], py[M * M]; // path coordinates

public:
    int start[2];
    int finish[2];

public:
    LeeAlgorithm();

    std::pair<int, Pos> out();

    void in(std::vector<int> tilesStatus, Pos startPos, Pos endPos);

    void find_start_end();

    bool lee(int ax, int ay, int bx, int by);

    bool lee();
};
