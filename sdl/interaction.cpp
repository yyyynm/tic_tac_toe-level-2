/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

/* ---------- Definitions ---------- */

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */
bool SDLInteraction::waitForQuit(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        throw QuitException();
    }
    return false;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction() {
}

/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction() {
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig& config) {
    // TODO:
    // - Khởi tạo các thành phần cần thiết cho input SDL
    // - Có thể reset event queue hoặc trạng thái input
    this->m_config = config;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); // dọn dẹp sự kiện
    SDL_StartTextInput(); // nhập input lớn hơn 9
}

/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout) {
    // Nếu có timeout cụ thể -> delay trực tiếp bằng SDL
    if (timeout > 0) {
        SDL_Delay(timeout);
        return;
    }

    bool waiting = true;
    SDL_Event event;

    // Vòng lặp chờ sự kiện từ người dùng
    while (waiting) {
        // SDL_WaitEvent sẽ block cho tới khi có event
        if (SDL_WaitEvent(&event)) {
            // Nếu người dùng đóng cửa sổ -> thoát game
            if (waitForQuit(event)) {
            }

            // tương tự
            // if (event.type == SDL_QUIT) {
            //     throw QuitException();
            // }

            // Nếu có tương tác (nhấn phím hoặc click chuột) -> kết thúc pause
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                waiting = false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */
bool SDLInteraction::selectSize(int* size) {
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            exit(0);
        }
        else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9){
                int digit = e.key.keysym.sym - SDLK_0;
                if(*size == -1){
                    *size = digit; 
                } 
                else{
                    *size = (*size) * 10 + digit; 
                }
                return false;
            }
            else if(e.key.keysym.sym == SDLK_BACKSPACE){
                if(*size > 9) *size /= 10;
                else *size = -1;
                return false; 
            }
            else if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER){
                if(*size >= 3 && *size <= 12){
                    return true; 
                } 
                else{
                    *size = -2; 
                    return false;
                }
            }
        }
    }
    return false; 
}


/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
    // TODO:
    // - Lấy input từ SDL
    // - Parse thành số nguyên
    // - Kiểm tra điều kiện (3 <= goal <= size)
    // - Trả về true nếu hợp lệ
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            exit(0);
        }
        else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9){
                int digit = e.key.keysym.sym - SDLK_0;
                if(*goal == -1){
                    *goal = digit; 
                } 
                else{
                    *goal = (*goal) * 10 + digit; 
                }
                return false;
            }
            else if(e.key.keysym.sym == SDLK_BACKSPACE){
                if(*goal > 9) *goal /= 10;
                else *goal = -1;
                return false; 
            }
            else if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER){
                if(*goal >= 3 && *goal <= size){
                    return true; 
                } 
                else{
                    *goal = -2; 
                    return false;
                }
            }
        }
    }
    return false; 
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    int choice = (int)(*mode);
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT) exit(0);
        else if(e.type == SDL_KEYDOWN){
            SDL_Keycode key = e.key.keysym.sym;
            if(key == SDLK_RETURN || key == SDLK_KP_ENTER){
                if(choice == 1){ *mode = GameMode::PVP; choice = -1; return true; }
                if(choice == 2){ *mode = GameMode::PVE; choice = -1; return true; }
                if(choice == 3){ *mode = GameMode::EVE; choice = -1; return true; }
                choice = -2;
            }
            else if(key == SDLK_BACKSPACE){
                if(choice > 9) choice /= 10;
                else choice = -1;
            }
            else if((key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_KP_0 && key <= SDLK_KP_9)){
                int digit = (key >= SDLK_0 && key <= SDLK_9) ? (key - SDLK_0) : (key - SDLK_KP_0);
                if(choice == -1) choice = digit; 
                else choice = (choice) * 10 + digit; 
            }
        }
    }
    *mode = (GameMode)choice;
    return false;
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 *   - levels: mảng lưu cấp độ bot.
 *   - index: vị trí bot cần gán.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    // TODO:
    // - Kiểm tra index hợp lệ (0 hoặc 1)
    // - Lấy input từ SDL
    // - Map sang BotLevel (EASY, MEDIUM, HARD)
    // - Trả về true nếu hợp lệ
    if(index < 0 || index > 1) return false;
    int choice = (int)levels[index];
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT) exit(0);
        else if(e.type == SDL_KEYDOWN){
            SDL_Keycode key = e.key.keysym.sym;
            if(key == SDLK_RETURN || key == SDLK_KP_ENTER){
                if(choice == 1){ levels[index] = BotLevel::EASY; choice = -1; return true; }
                if(choice == 2){ levels[index] = BotLevel::MEDIUM; choice = -1; return true; }
                if(choice == 3){ levels[index] = BotLevel::HARD; choice = -1; return true; }
                levels[index] = (BotLevel)(-2);
                return false;
            }
            else if(key == SDLK_BACKSPACE){
                if(choice > 9) choice /= 10;
                else choice = -1;
            }
            else if((key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_KP_0 && key <= SDLK_KP_9)){
                int digit = (key >= SDLK_0 && key <= SDLK_9) ? (key - SDLK_0) : (key - SDLK_KP_0);
                if(choice == -1 || choice == -2) choice = digit;
                else choice = (choice) * 10 + digit; 
            }
        }
    }
    if((int)levels[index] != -2){
        levels[index] = (BotLevel)choice; 
    }
    return false;
}


/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
    // TODO:
    // - Lắng nghe mouse click hoặc keyboard input
    // - Chuyển đổi tọa độ click thành (row, col)
    // - Validate phạm vi hợp lệ
    SDL_Event e;
    int minDimension = std::min(m_config.screenWidth, m_config.screenHeight);
    int drawArea = minDimension - (2 * m_config.boardPadding);
    int cellSize = drawArea / m_boardSize; 
    int startX = (m_config.screenWidth - drawArea) / 2;
    int startY = (m_config.screenHeight - drawArea) / 2;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT) exit(0);
        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            int relativeX = mouseX - startX;
            int relativeY = mouseY - startY;
            if(relativeX >= 0 && relativeY >= 0 && relativeX < drawArea && relativeY < drawArea){
                *col = relativeX / cellSize;
                *row = relativeY / cellSize;
                return true; 
            }
        }
    }
    return false;
}

/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
    // TODO:
    // - Dọn dẹp trạng thái, nếu có resource thì giải phóng
    SDL_StopTextInput();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}