/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"
#include <SDL2/SDL_image.h>

void SDLRenderer::renderText(const std::string& text, int x, int y, SDL_Color color){
    if(font == nullptr) return; 
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if(surface == nullptr) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer() {
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer() {
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */
void SDLRenderer::init(const RunConfig& config) {
    // read config
    this->m_config = config;
    int screenWidth = config.screenWidth;
    int screenHeight = config.screenHeight;

    // int boardPadding = config.boardPadding;
    // init(...)

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // load font
    font = TTF_OpenFont("assets/font.ttf", 28);
    if(font == nullptr){
        SDL_Log("Cảnh báo: Không thể load font.ttf! Lỗi: %s", TTF_GetError());
    }
    SDL_Surface* loadedSurface = IMG_Load("assets/background.jpg");
    if(loadedSurface == nullptr){
        loadedSurface = IMG_Load("../assets/background.jpg"); 
    }
    if(loadedSurface == nullptr){
        printf("Cảnh báo: Không thể tải ảnh nền ở cả 2 đường dẫn! Lỗi: %s\n", IMG_GetError());
    } 
    else{
        m_backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface); 
    }
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // dark background
    SDL_RenderClear(renderer);
    if(m_backgroundTexture != nullptr){
    SDL_RenderCopy(renderer, m_backgroundTexture, NULL, NULL);
}
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent() {
    SDL_RenderPresent(renderer);
}

/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(m_backgroundTexture != nullptr){
       SDL_RenderCopy(renderer, m_backgroundTexture, NULL, NULL);
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
       SDL_RenderFillRect(renderer, NULL);
    }
    std::string titleText = "";
    std::string optionsText = "";
    switch(selectType){
        case SelectType::TITLE_UI:
            titleText = std::format(">----- Tic-tac-toe [SDL v{}] -----<", VERSION);
            break;
        case SelectType::SIZE_UI:
            // TODO: display board size selection
            // Example: "Size Input (NxN, 3 <= N <= BOARD_N_MAX)"
            titleText = "PLEASE ENTER YOUR BOARD SIZE FROM 3 TO 12:";
            break;
        case SelectType::GOAL_UI:
            // TODO: display goal selection
            // Example: "Goal Input (3 - 5, goal <= size)"
            titleText = "PLEASE ENTER YOUR WINNING GOAL:";
            break;
        case SelectType::GAME_MODE_UI:
            // TODO: display game mode selection
            // Example: "(1) PvP | (2) PvE | (3) EvE"
            titleText = "PLEASE CHOOSE YOUR GAMEMODE:";
            optionsText = "(1) PvP  |  (2) PvE  |  (3) EvE";
            break;
        case SelectType::BOT_LEVEL_UI:
            // TODO: display bot level selection
            // Example: "(1) EASY | (2) MEDIUM | (3) HARD"
            titleText = "PLEASE CHOOSE YOUR BOT LEVEL:";
            optionsText = "(1) EASY  |  (2) MEDIUM  |  (3) HARD";
            break;
        case SelectType::PLAYER_UI:
            // TODO: display player move prompt
            // Example input format: "(row, col)"
            titleText = "ENTER YOUR MOVE (Click on the board):";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            // TODO: display multiple bot level selection
            // Example input format: "(bot1_level, bot2_level)"
            if(context == -3){
                titleText = "CHOOSE YOUR BOT 2 LEVEL:";
            }
            else{
                titleText = "CHOOSE YOUR BOT 1 LEVEL:";
            }
            optionsText = "(1) EASY  |  (2) MEDIUM  |  (3) HARD";
            break;
        default:
            break;
    }
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color whiteColor = {255, 255, 255, 255};
    if(!titleText.empty()){
        renderText(titleText, 50, 100, textColor);
    }
    if(!optionsText.empty()){
        renderText(optionsText, 50, 160, textColor);
    }
    std::string textToRender = "";
    SDL_Color yellowColor = {255, 255, 0, 255};
    if(selectType == SelectType::TITLE_UI){
        textToRender = "PRESS ENTER TO PLAY";
        renderText(textToRender, 350, 250, yellowColor);
    }
    else{
        SDL_Rect inputBox = {50, 240, 500, 50};
        SDL_SetRenderDrawColor(renderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a);
        SDL_RenderDrawRect(renderer, &inputBox);
        if(context == -1 || context == -2 || context == -3){
            textToRender = "YOUR INPUT: _";
        } 
        else{
            textToRender = "YOUR INPUT: " + std::to_string(context);
        }
        renderText(textToRender, 70, 252, yellowColor);
    }
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context){
    // TODO: Render thông báo lỗi
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(m_backgroundTexture != nullptr){
       SDL_RenderCopy(renderer, m_backgroundTexture, NULL, NULL);
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
       SDL_RenderFillRect(renderer, NULL);
    }
    std::string errorText = "[!] ERROR: "; 
    switch(selectType){
        case SelectType::SIZE_UI:
            errorText += "INVALID BOARD SIZE";
            break;
        case SelectType::BOT_LEVEL_UI:
            errorText += "INVALID BOT LEVEL";
            break;
        case SelectType::GAME_MODE_UI:
            errorText += "INVALID GAME MODE";
            break;
        case SelectType::GOAL_UI:
            errorText += "INVALID GOAL";
            break;
        case SelectType::PLAYER_UI:
            errorText += "INVALID MOVE";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            if(context == 0){
                errorText += "INVALID BOT 1 LEVEL";
            } 
            else if(context == 1){
                errorText += "INVALID BOT 2 LEVEL";
            }
            break;
        default:
            errorText += "ERROR (CODE: " + std::to_string(context) + ").";
            break;
    }
    SDL_Color redColor = {255, 0, 0, 255};
    renderText(errorText, 50, 300, redColor);
    renderPresent();
}


/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context){
    // TODO: Render thông báo thành công
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(m_backgroundTexture != nullptr){
       SDL_RenderCopy(renderer, m_backgroundTexture, NULL, NULL);
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
       SDL_RenderFillRect(renderer, NULL);
    }
    std::string successText = "[+] SUCCESS: "; 
    switch(selectType){
        case SelectType::SIZE_UI:
            successText += "BOARD SIZE APPROVED";
            break;
        case SelectType::BOT_LEVEL_UI:
            successText += "BOT LEVEL APPROVED";
            break;
        case SelectType::GAME_MODE_UI:
            successText += "GAME MODE APPROVED";
            break;
        case SelectType::GOAL_UI:
            successText += "GOAL APPROVED";
            break;
        case SelectType::PLAYER_UI:
            successText += "MOVE APPROVED";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            if(context ==0){
                successText += "BOT 1 LEVEL APPROVED";
            }
            if(context == 1){
                successText += "BOT 2 LEVEL APPROVED";
            }
            break;
        default:
            successText += "OPTION APPROVED";
            break;
    }
    SDL_Color greenColor = {0, 255, 0, 255}; 
    renderText(successText, 50, 350, greenColor);
    renderPresent();
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size){
    // TODO: Render board
    this->m_boardSize = size;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(m_backgroundTexture != nullptr){
       SDL_RenderCopy(renderer, m_backgroundTexture, NULL, NULL);
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
       SDL_RenderFillRect(renderer, NULL);
    }
    int padding = m_config.boardPadding; 
    int availableWidth = m_config.screenWidth - (2 * padding);
    int availableHeight = m_config.screenHeight - (2 * padding);
    int drawArea = std::min(availableWidth, availableHeight);
    int cellSize = drawArea / size;
    int actualBoardSize = cellSize * size; 
    int startX = (m_config.screenWidth - actualBoardSize) / 2;
    int startY = (m_config.screenHeight - actualBoardSize) / 2;
    SDL_Color gridColor = {0, 255, 0, 255}; 
    SDL_Color redColor = {255, 0, 0, 255};  
    SDL_Color blueColor = {0, 0, 255, 255}; 
    for (int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            int cellX = startX + (j * cellSize);
            int cellY = startY + (i * cellSize);
            drawRect(cellX, cellY, cellSize, cellSize, gridColor, false);
            char mark = board[i][j];
            if(mark != ' ' && mark != '\0'){
                std::string markStr(1, mark); 
                int textW = 0;
                int textH = 0;
                TTF_SizeText(font, markStr.c_str(), &textW, &textH);
                int textX = cellX + (cellSize - textW) / 2;
                int textY = cellY + (cellSize - textH) / 2;
                if(mark == 'X'){
                    renderText(markStr, textX, textY, redColor);
                } 
                else if(mark == 'O'){
                    renderText(markStr, textX, textY, blueColor);
                }
            }
        }
    }
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col) {
    // TODO: Highlight move
    int size = this->m_boardSize;
    int padding = m_config.boardPadding; 
    int availableWidth = m_config.screenWidth - (2 * padding);
    int availableHeight = m_config.screenHeight - (2 * padding);
    int drawArea = std::min(availableWidth, availableHeight);
    int cellSize = drawArea / size;
    int actualBoardSize = cellSize * size; 
    int startX = (m_config.screenWidth - actualBoardSize) / 2;
    int startY = (m_config.screenHeight - actualBoardSize) / 2;
    int cellX = startX + (col * cellSize);
    int cellY = startY + (row * cellSize);
    SDL_Color yellowHighlight = {255, 255, 0, 255}; 
    drawRect(cellX, cellY, cellSize, cellSize, yellowHighlight, false);
    drawRect(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2, yellowHighlight, false);
    drawRect(cellX + 2, cellY + 2, cellSize - 4, cellSize - 4, yellowHighlight, false);
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove() {
    // TODO: Render invalid move message
    SDL_Color redColor = {255, 0, 0, 255};
    int textX = 20;
    int textY = m_config.screenHeight - 80;
    renderText("ERROR! PLEASE ENTER A VALID MOVE", textX, textY, redColor);
    renderPresent();
}

/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot) {
    // TODO: Render player info
    std::string infoText = "";
    if(is_bot){
        infoText = "BOT (PLAYER " + std::to_string(player + 1) + ") IS THINKING...";
    } else {
        infoText = "PLAYER " + std::to_string(player + 1) + "'S TURN. PLEASE MOVE!";
    }
    SDL_Color purpleColor = {200, 0, 255, 255}; 
    renderText(infoText, 20, 20, purpleColor);
    renderPresent();
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: Render result
    std::string resultText = "";
    SDL_Color textColor;
    if(winner == -1){
        resultText = "DRAW (HOA NHAU)!";
        textColor = {200, 200, 200, 255};
    } 
    else{
        textColor = {255, 215, 0, 255};
        if(is_bot){
            resultText = "PLAYER " + std::to_string(winner + 1) + " (BOT) WINS!";
        } 
        else{
            resultText = "PLAYER " + std::to_string(winner + 1) + " WINS!";
        }
    }
    renderText(resultText, 20, 20, textColor);
    if(winLine != nullptr && !winLine->cells.empty()){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        int padding = m_config.boardPadding;
        int drawArea = m_config.screenHeight - (2 * padding);
        int cellSize = drawArea / m_boardSize;
        int offsetX = (m_config.screenWidth - (m_boardSize * cellSize)) / 2;
        int offsetY = (m_config.screenHeight - (m_boardSize * cellSize)) / 2;
        int startRow = winLine->cells.front().first;
        int startCol = winLine->cells.front().second;
        int endRow = winLine->cells.back().first;
        int endCol = winLine->cells.back().second;
        int x1 = offsetX + (startCol * cellSize) + (cellSize / 2);
        int y1 = offsetY + (startRow * cellSize) + (cellSize / 2);
        int x2 = offsetX + (endCol * cellSize) + (cellSize / 2);
        int y2 = offsetY + (endRow * cellSize) + (cellSize / 2);
        for(int i = -2; i <= 2; i++){
            SDL_RenderDrawLine(renderer, x1 + i, y1, x2 + i, y2);
            SDL_RenderDrawLine(renderer, x1, y1 + i, x2, y2 + i);
        }
    }
    renderPresent(); 
    SDL_Event e;
    bool is_waiting = true;
    while (is_waiting) {
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                is_waiting = false;
            }
        }
        SDL_Delay(16); 
    }
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult& gameResult) {
    // TODO: Print result
   std::cout << gameResult.winner << " " << gameResult.turns << std::endl;
}

/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close() {
    if(m_backgroundTexture != nullptr){
        SDL_DestroyTexture(m_backgroundTexture);
        m_backgroundTexture = nullptr;
    }
    if(font){
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();   
}