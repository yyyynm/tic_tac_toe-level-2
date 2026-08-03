/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            board[i][j]='-';
        }
    }
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    if(row >= 0 and row < size and col >=0 and col < size and board[row][col]=='-') return true;
    return false;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    board[row][col] = symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    // TODO: implement
    if(x<0 or x>=size or y<0 or y>=size) return true;
    else if(board[x][y]=='-') return true;
    else if(board[x][y]==symbol) return true;
    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    for(int i = 0; i<size; i++){
        int counts = 0;
        for(int j = 0; j<size; j++){
            if(board[i][j]==symbol) counts++;
            else counts = 0;
            if(counts==goal and rule == EndRule::OPEN_TWO){
                if(isEmptyHead(board, size, i, j+1, symbol) and isEmptyHead(board, size, i, j-goal, symbol)) return true;
            }
            else if(counts == goal and rule == EndRule::NONE) return true;
            else if(counts == goal and rule == EndRule::OPEN_ONE){
                if(isEmptyHead(board, size, i, j+1, symbol) or isEmptyHead(board, size, i, j-goal, symbol)) return true;
            }
        }
    }

    for(int i = 0; i<size; i++){
        int counts = 0;
        for(int j = 0; j<size; j++){
            if(board[j][i]==symbol) counts++;
            else counts = 0;
            if(counts==goal and rule == EndRule::OPEN_TWO){
                if(isEmptyHead(board, size, j+1, i, symbol) and isEmptyHead(board, size, j-goal, i, symbol)) return true;
            }
            else if(counts == goal and rule == EndRule::NONE) return true;
            else if(counts == goal and rule == EndRule::OPEN_ONE){
                if(isEmptyHead(board, size, j+1, i, symbol) or isEmptyHead(board, size, j-goal, i, symbol)) return true;
            }
        }
    }

    int count_chinh = 0;
    for(int i = 0; i<size; i++){
        if(board[i][i]==symbol) count_chinh++;
        else count_chinh = 0;
        if(count_chinh==goal and rule == EndRule::OPEN_TWO){
                if(isEmptyHead(board, size, i+1, i+1, symbol) and isEmptyHead(board, size, i-goal, i-goal, symbol)) return true;
        }
        else if(count_chinh == goal and rule == EndRule::NONE) return true;
        else if(count_chinh == goal and rule == EndRule::OPEN_ONE){
            if(isEmptyHead(board, size, i+1, i+1, symbol) or isEmptyHead(board, size, i-goal, i-goal, symbol)) return true;
        }
    }

    int count_phu = 0;
    for(int i = size-1; i>=0; i--){
        if(board[size-i-1][i]==symbol) count_phu++;
        else count_phu =0;
        if(count_phu==goal and rule == EndRule::OPEN_TWO){
            if(isEmptyHead(board,size,size-i,i-1,symbol) and isEmptyHead(board,size,size-i-1-goal,i+goal,symbol)) return true;
        }   
        else if(count_phu == goal and rule == EndRule::NONE) return true;
        else if(count_phu == goal and rule == EndRule::OPEN_ONE){
            if(isEmptyHead(board,size,size-i,i-1,symbol) or isEmptyHead(board,size,size-i-1-goal,i+goal,symbol)) return true;
        }
    }
    return false;
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if(board[i][j]=='-') return false;
        }
    }
    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    for(int i = 0; i<size; i++){
        int counts = 0;
        for(int j = 0; j<size; j++){
            if(board[i][j]==symbol) counts++;
            else counts = 0;
            if(counts==goal and rule == EndRule::OPEN_TWO){
                if(isEmptyHead(board, size, i, j+1, symbol) and isEmptyHead(board, size, i, j-goal, symbol)){
                    WinLine line;
                    int row = i;
                    int col = j-goal+1;
                    while(col<=j){
                        line.cells.push_back({row,col});
                        col++;
                    }
                    return line;
                }
            }
           
        }
    }

    for(int i = 0; i<size; i++){
        int counts = 0;
        for(int j = 0; j<size; j++){
            if(board[j][i]==symbol) counts++;
            else counts = 0;
            if(counts==goal and rule == EndRule::OPEN_TWO){
                if(isEmptyHead(board, size, j+1, i, symbol) and isEmptyHead(board, size, j-goal, i, symbol)){
                    WinLine line;
                    int row = j-goal+1;
                    int col = i;
                    while(row<=j){
                        line.cells.push_back({row,col});
                        row++;
                    }
                    return line;
                }
            }
        }
    }

    int count_chinh = 0;
    for(int i = 0; i<size; i++){
        if(board[i][i]==symbol) count_chinh++;
        else count_chinh = 0;
        if(count_chinh==goal and rule == EndRule::OPEN_TWO){
                if(isEmptyHead(board, size, i+1, i+1, symbol) and isEmptyHead(board, size, i-goal, i-goal, symbol)){
                    WinLine line;
                    int row = i-goal+1;
                    int col = i-goal+1;
                    while(row<=i && col<=i){
                        line.cells.push_back({row,col});
                        row++;
                        col++;
                    }
                    return line;
                }
        }
    }

    int count_phu = 0;
    for(int i = size-1; i>=0; i--){
        if(board[size-i-1][i]==symbol) count_phu++;
        else count_phu =0;
        if(count_phu==goal and rule == EndRule::OPEN_TWO){
            if(isEmptyHead(board,size,size-i,i-1,symbol) and isEmptyHead(board,size,size-i-1-goal,i+goal,symbol)){
                WinLine line;
                int row = size-i-1;
                int col = i;
                while(row>=size-i-goal && col<=i+goal-1){
                    line.cells.push_back({row,col});
                    row--;
                    col++;
                }
                return line;
            }
        }   
    }
    return std::nullopt;
}