/**
 * Bot LV2 cpp implementation
 *
 */

#include "bot_lv2.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel2.
 */
BotLevel2::BotLevel2(const BotLevel& _level, const char& _symbol)
    : BotLevel1(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel2.
 */
BotLevel2::~BotLevel2() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot MEDIUM.
 *
 * TODO:
 *   - Gọi heuristic để chọn nước đi
 */
pII BotLevel2::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    // TODO: gọi simple_heuristic với symbol và op_symbol
    return simple_heuristic(board,size,goal,symbol,op_symbol);
}

/**
 * Mô tả:
 *   Heuristic đơn giản:
 *   - Ưu tiên thắng
 *   - Chặn đối thủ
 *   - Fallback random
 *
 * TODO:
 *   - Bước 1: kiểm tra nước thắng
 *   - Bước 2: kiểm tra nước chặn
 *   - Bước 3: chọn random nếu không có
 */
pII BotLevel2::simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
    // TODO: implement heuristic
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if(board[i][j] =='-'){
                board[i][j] = botSymbol;
                if(Logic::checkWin(board, size, botSymbol, goal,EndRule::OPEN_TWO)){
                    board[i][j]='-';
                    return{i,j};
                }
                board[i][j] = '-';
            }
        }
    }
    for(int i = 0; i<size; i++){
        for(int j =0; j<size; j++){
            if(board[i][j]=='-'){
                board[i][j]=playerSymbol;
                if(Logic::checkWin(board, size, playerSymbol, goal,EndRule::OPEN_TWO)){
                    board[i][j]='-';
                    return {i,j};
                }
                board[i][j]='-';
            }
        }
    }
    if(size%2!=0 and board[size/2][size/2]=='-') return {size/2, size/2};
    else{
        std::vector<pII> v;
        if(board[size/2][size/2]=='-') v.push_back({size/2,size/2});
        if(board[(size/2)-1][(size/2)-1]=='-') v.push_back({(size/2)-1,(size/2)-1});
        if(board[(size/2)-1][(size/2)]=='-') v.push_back({(size/2)-1,size/2});
        if(board[size/2][(size/2)-1]=='-') v.push_back({size/2,(size/2)-1});
        if(v.size()!=0){
            int random_index = rand()%v.size();
            return v[random_index];
        }
    }
    if(board[0][0]=='-') return {0,0};
    else if(board[0][size-1]=='-') return {0,size-1};
    else if(board[size-1][0]=='-') return {size-1,0};
    else if(board[size-1][size-1]=='-') return {size-1,size-1};
    for(int i = 1; i<size-1; i++){
        if(board[0][i]=='-') return {0,i};
        else if(board[i][0]=='-') return {i,0};
        else if(board[size-1][i]=='-') return {size-1,i};
        else if(board[i][size-1]=='-') return {i,size-1};
    }
    // fallback
    return random_pick(board, size);
}