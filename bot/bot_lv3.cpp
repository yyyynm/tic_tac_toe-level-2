/**
 * Bot LV3 cpp implementation
 *
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel3.
 */
BotLevel3::BotLevel3(const BotLevel& _level, const char& _symbol)
    : BotLevel2(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel3.
 */
BotLevel3::~BotLevel3() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot HARD.
 *
 * TODO:
 *   - Bước 1: Gọi hard_level để tìm nước đi tối ưu
 *   - Bước 2: Nếu không tìm được → fallback về logic của BotLevel2
 */
pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    // TODO: gọi hard_level
    // TODO: fallback nếu cần
    return hard_level(board,size,goal);
}

/**
 * Mô tả:
 *   Logic nâng cao cho bot HARD.
 *   Có thể sử dụng:
 *   - minimax
 *   - alpha-beta pruning
 *   - heuristic scoring
 *
 * TODO:
 *   - Bước 1: Duyệt các nước đi hợp lệ
 *   - Bước 2: Đánh giá từng nước (score)
 *   - Bước 3: Chọn nước có score tốt nhất
 *   - Trường hợp biên: không còn nước đi
 */

 int evaluateDirection(const char board[][BOARD_N_MAX], int goal, int size, int x, int y, int dx, int dy, char symbol){
    int count = 1;
    int block = 0;
    int step = 1;
    while(true){
        int nx = x + step*dx;
        int ny = y + step*dy;
        if(nx < 0 || nx >= size || ny < 0 || ny >= size){
            block ++;
            break;
        }
        if(board[nx][ny]!=symbol && board[nx][ny]!='-'){
            block ++;
            break;
        }
        if(board[nx][ny]=='-') break;
        count++;
        step++;
    }
    step = 1;
    while(true){
        int nx = x - step*dx;
        int ny = y - step*dy;
        if(nx < 0 || nx >= size || ny < 0 || ny >= size){
            block ++;
            break;
        }
        if(board[nx][ny]!=symbol && board[nx][ny]!='-'){
            block ++;
            break;
        }
        if(board[nx][ny]=='-') break;
        count++;
        step++;
    }
    if (count >= goal) return 100000;
    if (count == goal-1 && block == 0) return 10000;
    if (count == goal-1 && block == 1) return 1000;
    if (count == goal-2 && block == 0) return 1000;
    if (count == goal-2 && block == 1) return 100;
    if (count == 2 && block == 0) return 10;
    return 0;
}

pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal) {
    // TODO: implement thuật toán nâng cao
    int dx[4] = {0,1,1,1};
    int dy[4] = {1,0,1,-1};
    int best_score = -1;
    pII best_move = {size / 2, size / 2};
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if(board[i][j]=='-'){
                int score = 0;
                for( int dir =0; dir<4; dir++){
                    int botScore = evaluateDirection(board,goal,size,i,j,dx[dir],dy[dir],symbol);
                    int playerScore = evaluateDirection(board,goal,size,i,j,dx[dir],dy[dir],op_symbol);
                    score += botScore+playerScore;
                }
                if(best_score <= score){
                    best_score = score;
                    best_move ={i,j};
                }
            }
        }
    }
    return best_move;

    // fallback
    return random_pick(board, size);
}