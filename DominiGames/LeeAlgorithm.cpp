#include "LeeAlgorithm.h"

LeeAlgorithm::LeeAlgorithm() {
    memset(&arr, 0, sizeof(arr));
    memset(&px, 0, sizeof(px));
    memset(&py, 0, sizeof(py));
    len = 0;
    start[0] = 0;
    start[1] = 0;
    finish[0] = 0;
    finish[1] = 0;
}

std::pair<int, Pos> LeeAlgorithm::out()
{
    Pos movePos{-1, -1};
    for (int y = 0; y < M; y++)
    {
        for (int x = 0; x < M; x++)
        {
            if (arr[y][x] == -1)
            {
                if (x - 1 >= 0 && arr[y][x - 1] == START ||  // left
                    x + 1 < M  && arr[y][x + 1] == START ||  // right
                    y - 1 >= 0 && arr[y - 1][x] == START ||  // up
                    y + 1 < M  && arr[y + 1][x] == START)    // down
                    movePos = {y, x};
            }
        }
    }

    int totalLength = len != 0 ? len - WAVE_START : 0;

    return {totalLength, movePos};
}

void LeeAlgorithm::in(std::vector<int> tilesStatus, Pos startPos, Pos endPos) {
    int* tmp = tilesStatus.data();
    int count = 0;
    for (auto& y : arr)
    {
        for (int& x : y)
        {
            x = tmp[count];
            ++count;
        }
    }
    arr[startPos.x][startPos.y] = START;
    arr[endPos.x][endPos.y] = FINISH;
}

void LeeAlgorithm::find_start_end() {
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < M; x++) {
            if (arr[y][x] == START) {
                start[0] = y;
                start[1] = x;
                arr[y][x] = BLANK;
            } else if (arr[y][x] == FINISH) {
                finish[0] = y;
                finish[1] = x;
                arr[y][x] = BLANK;
            }
        }
    }
}

bool LeeAlgorithm::lee(int ax, int ay, int bx, int by) {
    int dx[4] = {1, 0, -1, 0};  // right   0    left   0;
    int dy[4] = {0, 1, 0, -1};  //    0   up     0    down
    int d, x, y, k;
    bool stop;

    d = WAVE_START;
    arr[ay][ax] = WAVE_START;
    do {
        stop = true; //pretend that all free cells was marked
        for (y = 0; y < M; y++) {
            for (x = 0; x < M; x++) {
                if (arr[y][x] == d) { // cell (x, y) already marked as "d"
                    for (k = 0; k < 4; k++) {// check "neighbours"
                        int iy = y + dy[k], ix = x + dx[k];
                        if (iy >= 0 && iy < M && ix >= 0 && ix < M &&
                            arr[iy][ix] == BLANK) {
                            stop = false;
                            arr[iy][ix] = d + 1;  // found not marked
                        }
                    }
                }
            }
        }
        d++;
    } while (!stop && by == finish[0] && bx == finish[1]);

    if (arr[finish[0]][finish[1]] == BLANK) return false; // path have not found

    //rebuilding path
    len = arr[finish[0]][finish[1]];
    x = finish[1];
    y = finish[0];
    d = len;
    while (d > WAVE_START) {
        px[d] = x;
        py[d] = y;
        d--;
        for (k = 0; k < 4; k++) {
            int iy = y + dy[k], ix = x + dx[k];
            if (iy >= 0 && iy < M && ix >= 0 && ix < M &&
                arr[iy][ix] == d) {
                x = x + dx[k];
                y = y + dy[k]; // move to the start point
                break;
            }
        }
    }
    px[0 + d] = ax;
    py[0 + d] = ay;

    //drawing the path
    for (int i = WAVE_START; i <= len; i++) {
        arr[py[i]][px[i]] = -1;
    }
    arr[start[0]][start[1]] = START;
    arr[finish[0]][finish[1]] = FINISH;

    //restructuring LeeAlgorithm
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < M; x++) {
            if (arr[y][x] >= WAVE_START) arr[y][x] = 0;

        }
    }

    return true;
}

bool LeeAlgorithm::lee() {
    return lee(start[1], start[0], finish[1], finish[0]);
}