/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>
#include <fstream>
#include "../game/setup.h"
#include "../utils/config.h"
#include <sstream>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<bool> isCalculating(false);

void continuousDots() {
    while (isCalculating) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    MSG,
};

/**
 * Convert logging level to printable string.
 */
inline std::string levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARNING:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::MSG:
            return "";
        default:
            return "UNKNOWN";
    }
}
inline static Level min_level = Level::DEBUG;

// Level Color
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";
const std::string PURPLE  = "\033[95m";            
const std::string ORANGE  = "\033[38;5;214m";  

/**
 * Return terminal color code corresponding to a log level.
 */
inline std::string getColor(Level level) {
    switch (level) {
        case Level::DEBUG:
            return BLUE;
        case Level::INFO:
            return GREEN;
        case Level::WARNING:
            return YELLOW;
        case Level::ERROR:
            return RED;
        default:
            return RESET;
    }
}

// Static Variables
inline static std::ofstream log_file;
inline static bool write_to_file = false;
inline static bool is_judge_mode = false;


/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
    // TODO: init state
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    // TODO: cleanup nếu cần
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    // TODO: init renderer theo config
    return;
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    // TODO: clear screen
    std::cout << "\n"
              << "\x1B[2J\x1B[H"
              << "\n";
    return;
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: render menu theo loại
    std::cout << PURPLE;
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
            // TODO: display board size selection
            // Example: "Size Input (NxN, 3 <= N <= BOARD_N_MAX)"
            std::cout << "PLEASE ENTER YOUR BOARD SIZE FROM 3 TO 12" << std::endl;
            break;

        case SelectType::GOAL_UI:
            // TODO: display goal selection
            // Example: "Goal Input (3 - 5, goal <= size)"
            std::cout << "PLEASE ENTER YOUR WINNING GOAL FROM 3 TO SIZE" << std::endl;
            break;

        case SelectType::GAME_MODE_UI:
            // TODO: display game mode selection
            // Example: "(1) PvP | (2) PvE | (3) EvE"
            std::cout << "PLEASE CHOOSE YOUR GAMEMODE" << std::endl;
            std::cout << "(1) PvP | (2) PvE | (3) EvE" << std::endl;
            break;

        case SelectType::BOT_LEVEL_UI:
            // TODO: display bot level selection
            // Example: "(1) EASY | (2) MEDIUM | (3) HARD"
            std::cout << "  PLEASE CHOOSE YOUR BOT LEVEL " << std::endl;
            std::cout << "(1) EASY | (2) MEDIUM | (3) HARD" << std::endl;
            break;

        case SelectType::PLAYER_UI:
            // TODO: display player move prompt
            // Example input format: "(row, col)"
            std::cout << "ENTER YOUR MOVE" << std::endl;
            break;

        case SelectType::MUL_BOT_LEVEL_UI:
            if(context == -1 || context == 0){
                std::cout << "     CHOOSE YOUR BOT 1 LEVEL    " << std::endl;
                std::cout << "(1) EASY | (2) MEDIUM | (3) HARD" << std::endl;
            }
            else if(context == -3 || context == 1){
                std::cout << "     CHOOSE YOUR BOT 2 LEVEL    " << std::endl;
                std::cout << "(1) EASY | (2) MEDIUM | (3) HARD" << std::endl;
            }
            else {
                std::cout << "     CHOOSE BOT LEVEL    " << std::endl;
                std::cout << "(1) EASY | (2) MEDIUM | (3) HARD" << std::endl;
            }
            break;
        default:
            break;
    }
    std::cout << RESET;
    return;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: render invalid message
    std::cout << PURPLE;
    std::cout << "\n[!] ERROR: "; 
    
    switch (selectType) {
        case SelectType::SIZE_UI:
            std::cout << "INVALID BOARD SIZE" << std::endl;
            break;
        case SelectType::BOT_LEVEL_UI:
            std::cout << "INVALID BOT LEVEL" << std::endl;
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "INVALID GAME MODE" << std::endl;
            break;
        case SelectType::GOAL_UI:
            std::cout << "INVALID GOAL" << std::endl;
            break;
        case SelectType::PLAYER_UI:
            std::cout << "INVALID MOVE" << std::endl;
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            if(context == 0){
                std::cout << "INVALID BOT 1 LEVEL" << std::endl;
            }
            if(context == 1){
                std::cout << "INVALID BOT 2 LEVEL" << std::endl;
            }
            break;
        default:
            std::cout << "ERROR (CODE: " << context << ")." << std::endl;
            break;
    }
    std::cout << "\n";
    std::cout << RESET;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: render success message
    std::cout << PURPLE;
    std::cout << "\n[+] SUCCESS: "; 
    
    switch (selectType) {
        case SelectType::SIZE_UI:
            std::cout << "BOARD SIZE APPROVED" << std::endl;
            break;
        case SelectType::BOT_LEVEL_UI:
            std::cout << "BOT LEVEL APPROVED" << std::endl;
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "GAME MODE APPROVED" << std::endl;
            break;
        case SelectType::GOAL_UI:
            std::cout << "GOAL APPROVED" << std::endl;
            break;
        case SelectType::PLAYER_UI:
            std::cout << "MOVE APPROVED" << std::endl;
            break;
        default:
            std::cout << "OPTION APPROVED" << std::endl;
            break;
    }
    std::cout << "\n";
    std::cout << RESET;
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO: render board
    std::stringstream ss;
    ss << "\n    "; 
    for(int j = 0; j < size; j++){
        if(j < 10) ss << " " << j << "  "; 
        else ss << " " << j << " ";       
    }
    ss << "\n";
    ss << GREEN << "   +";
    for(int j = 0; j < size; j++) ss << "---+";
    ss << RESET << "\n"; 
    for(int i = 0; i < size; i++){
        if(i < 10) ss << " " << i << " ";
        else ss << i << " ";
        ss << GREEN << "|" << RESET;
        for(int j = 0; j < size; j++){
            if(board[i][j] == ' ' || board[i][j] == '\0'){
                ss << "   " << GREEN << "|" << RESET; 
            } 
            else if (board[i][j] == 'X') {
                ss << RED << " X " << GREEN << "|" << RESET;
            }
            else if (board[i][j] == 'O') {
                ss << BLUE << " O " << GREEN << "|" << RESET; 
            }
            else {
                ss << " " << board[i][j] << " " << GREEN << "|" << RESET; 
            }
        }
        ss << "\n";
        ss << GREEN << "   +";
        for(int j = 0; j < size; j++) ss << "---+";
        ss << RESET << "\n";
    }
    std::cout << ss.str() << std::endl;
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    // TODO: render player info
    std::cout << PURPLE;
    if(is_bot) {
        for(int loop = 0; loop < 2; loop++) {
            std::cout << "\r" << PURPLE << "BOT (PLAYER " << player + 1 << ") IS THINKING   " << RESET << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
            std::cout << "\r" << PURPLE << "BOT (PLAYER " << player + 1 << ") IS THINKING.  " << RESET << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
            std::cout << "\r" << PURPLE << "BOT (PLAYER " << player + 1 << ") IS THINKING.. " << RESET << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
            std::cout << "\r" << PURPLE << "BOT (PLAYER " << player + 1 << ") IS THINKING..." << RESET << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        std::cout << std::endl;
    } else {
        std::cout << PURPLE << "PLAYER " << player + 1 << " IS THINKING..." << RESET << std::endl;
    }
    std::cout << RESET;
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    // TODO: render move
    std::cout << PURPLE;
    std::stringstream ss;
    ss << "MOVE PLACED AT (" << row << ',' << col << ')';
    std::cout << ss.str();
    std::cout << RESET;
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    // TODO: render invalid move message
    std::cout << PURPLE;
    std::cout << "ERROR! PLEASE ENTER A VALID MOVE" << std::endl;
    std::cout << RESET;
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: render result
    std::cout << PURPLE;
    std::stringstream ss;
    if(winner == -1){
        std::cout << "HOA";
    }
    else{
        if(is_bot) ss << "PLAYER" << " " << winner + 1 << "(BOT)" << " " << "WINS";
        else ss << "PLAYER" << " " << winner + 1 << " " << "WINS";
    }
    std::cout << ss.str() << std::endl;
    std::cout << RESET;
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    // TODO: print result đơn giản
    std::cout << gameResult.winner << " " << gameResult.turns << std::endl;
}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    // TODO: cleanup renderer
}