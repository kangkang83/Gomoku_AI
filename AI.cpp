#include "AI.h"
#include <algorithm>
#include <vector>

using namespace std;

AI::AI() {}
// Hàm chính tìm nước đi tốt nhất
Move AI::findBestMove(Board& board) {
    Move bestMove;
    bestMove.score = -1000000000000000000LL; // Khởi tạo điểm rất thấp
    bestMove.x = -1;
    bestMove.y = -1;

    // Lấy các ô có thể đi (chỉ xét ô xung quanh các quân đã đánh)
    vector<pair<int, int>> possibleMoves = board.getEmptyNeighbors();

    // Sắp xếp các nước đi dựa trên heuristic đánh giá nhanh
    // Move Ordering: Giúp Alpha-Beta cắt tỉa tốt hơn
    vector<pair<long long, pair<int, int>>> rankedMoves;
    for (auto p : possibleMoves) {
        long long moveScore = evaluateMove(board, p.first, p.second, BOT);
        rankedMoves.push_back({moveScore, p});
    }

    // Sắp xếp giảm dần điểm số (nước đi tốt nhất kiểm tra trước)
    sort(rankedMoves.rbegin(), rankedMoves.rend());

    // Duyệt qua các nước đi đã sắp xếp
    for (auto item : rankedMoves) {
        pair<int, int> p = item.second;
        board.move(p.first, p.second, BOT);
        
        // Gọi Minimax
        long long score = minimax(board, 5, -1000000000000000000LL, 1000000000000000000LL, false); // Tang do sau len 5 cho thong minh hon
        
        board.undo(p.first, p.second);

        if (score > bestMove.score) {
            bestMove.score = score;
            bestMove.x = p.first;
            bestMove.y = p.second;
        }
    }
    return bestMove;
}

// Thuật toán Minimax với Alpha-Beta Pruning
long long AI::minimax(Board& board, int depth, long long alpha, long long beta, bool isMaximizing) {
    // Nếu đạt độ sâu giới hạn hoặc bàn cờ đầy
    if (depth == 0) {
        return evaluate(board, BOT);
    }

    vector<pair<int, int>> possibleMoves = board.getEmptyNeighbors();
    if (possibleMoves.empty()) return 0;

    if (isMaximizing) {
        long long maxEval = -1000000000000000000LL;

        // Sắp xếp nước đi cho BOT (BOT muốn điểm cao nhất)
        vector<pair<long long, pair<int, int>>> rankedMoves;
        for (auto p : possibleMoves) {
            long long moveScore = evaluateMove(board, p.first, p.second, BOT);
            rankedMoves.push_back({moveScore, p});
        }
        sort(rankedMoves.rbegin(), rankedMoves.rend());

        for (auto item : rankedMoves) {
            pair<int, int> p = item.second;
            
            // Kiểm tra thắng ngay lập tức để cắt tỉa
            if (board.checkWin(p.first, p.second, BOT)) {
                return WIN_SCORE;
            }

            board.move(p.first, p.second, BOT);
            long long eval = minimax(board, depth - 1, alpha, beta, false);
            board.undo(p.first, p.second);
            
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break; // Cắt tỉa Beta
        }
        return maxEval;
    } else {
        long long minEval = 1000000000000000000LL;

        // Sắp xếp nước đi cho PLAYER (PLAYER muốn điểm thấp nhất cho BOT, tức là tốt nhất cho PLAYER)
        vector<pair<long long, pair<int, int>>> rankedMoves;
        for (auto p : possibleMoves) {
            long long moveScore = evaluateMove(board, p.first, p.second, PLAYER); // Đánh giá cho Player
            rankedMoves.push_back({moveScore, p});
        }
        sort(rankedMoves.rbegin(), rankedMoves.rend()); // Player cũng sẽ chọn nước đi tốt nhất cho họ

        for (auto item : rankedMoves) {
            pair<int, int> p = item.second;
            
            // Kiểm tra đối thủ thắng ngay lập tức
            if (board.checkWin(p.first, p.second, PLAYER)) return LOSE_SCORE;

            board.move(p.first, p.second, PLAYER);
            long long eval = minimax(board, depth - 1, alpha, beta, true);
            board.undo(p.first, p.second);
            
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break; // Cắt tỉa Alpha
        }
        return minEval;
    }
}

// Hàm đánh giá thế cờ (Heuristic)
// Hàm đánh giá thế cờ (Heuristic) cho toàn bộ bàn cờ
long long AI::evaluate(Board& board, int player) {
    long long aiScore = 0;
    long long playerScore = 0;
    
    // Duyệt qua tất cả các hướng để đếm chuỗi quân cờ: Ngang, Dọc, Chéo chính, Chéo phụ
    int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board.grid[r][c] != EMPTY) {
                // Chỉ duyệt theo 4 hướng duy nhất từ mỗi ô để tránh lặp lại (Double counting)
                // Ví dụ: Chuỗi A-B-C sẽ chỉ được đếm khi duyệt từ A theo hướng sang phải, 
                // không đếm lại khi duyệt từ B hay C.
                for (int d = 0; d < 4; d++) {
                    int count = 0;
                    int block = 0;
                    int curPlayer = board.grid[r][c];
                    
                    // Kiểm tra xem ô trước đó theo hướng này có phải là quân cùng màu không.
                    // Nếu CÓ, nghĩa là chuỗi này đã được xử lý ở ô trước đó rồi -> Bỏ qua (Continue)
                    int prevR = r - dirs[d][0];
                    int prevC = c - dirs[d][1];
                    if (prevR >= 0 && prevR < BOARD_SIZE && prevC >= 0 && prevC < BOARD_SIZE && board.grid[prevR][prevC] == curPlayer) {
                        continue; 
                    }

                    // Bắt đầu đếm chuỗi liên tiếp từ ô hiện tại (r, c)
                    int tempR = r;
                    int tempC = c;
                    while (tempR >= 0 && tempR < BOARD_SIZE && tempC >= 0 && tempC < BOARD_SIZE && board.grid[tempR][tempC] == curPlayer) {
                        count++;
                        tempR += dirs[d][0];
                        tempC += dirs[d][1];
                    }

                    // Kiểm tra xem chuỗi có bị chặn ở đầu TRƯỚC (Previous) không
                    if (prevR >= 0 && prevR < BOARD_SIZE && prevC >= 0 && prevC < BOARD_SIZE && board.grid[prevR][prevC] != EMPTY) block++; // Bị chặn bởi quân đối phương
                    else if (prevR < 0 || prevR >= BOARD_SIZE || prevC < 0 || prevC >= BOARD_SIZE) block++; // Bị chặn bởi mép bàn cờ

                    // Kiểm tra xem chuỗi có bị chặn ở đầu SAU (Next) không
                    if (tempR >= 0 && tempR < BOARD_SIZE && tempC >= 0 && tempC < BOARD_SIZE && board.grid[tempR][tempC] != EMPTY) block++; // Bị chặn bởi quân đối phương
                    else if (tempR < 0 || tempR >= BOARD_SIZE || tempC < 0 || tempC >= BOARD_SIZE) block++; // Bị chặn bởi mép bàn cờ

                    // Tính điểm cho chuỗi này dựa trên độ dài và số đầu bị chặn
                    long long score = evaluateLine(count, block, curPlayer == BOT);
                    
                    if (curPlayer == BOT) {
                        aiScore += score;
                    } else {
                        playerScore += score;
                    }
                }
            }
        }
    }
    
    // CHIẾN THUẬT: Tấn công hay Phòng thủ?
    // Công thức: Score = Điểm Tấn Công - (Hệ số * Điểm Phòng Thủ)
    // Hệ số 15.0: RẤT CAO. Nghĩa là AI sợ thua hơn là muốn thắng.
    // Nếu AI thấy người chơi có nguy cơ thắng (playerScore cao), nó sẽ bị trừ điểm rất nặng,
    // dẫn đến việc Minimax sẽ chọn nước đi nào làm giảm playerScore của đối thủ (tức là chặn lại).
    return aiScore - (long long)(playerScore * 15.0); 
}

// Bảng điểm Heuristic 
// Bảng điểm Heuristic - Đánh giá dựa trên số lượng quân liên tiếp và số đầu bị chặn
long long AI::evaluateLine(int count, int blocks, int currentTurn) {
    // TRƯỜNG HỢP 1: Bị chặn cả 2 đầu
    // Nếu bị chặn 2 đầu và chưa đủ 5 quân thì chuỗi này coi như vô dụng vì không thể phát triển thành 5 được.
    if (blocks == 2 && count < 5) return 0;

    // TRƯỜNG HỢP 2: Đã đạt 5 quân (CHIẾN THẮNG)
    // Đây là ưu tiên cao nhất, điểm số tuyệt đối.
    if (count == 5) return WIN_SCORE; // 100,000,000

    // TRƯỜNG HỢP 3: Có 4 quân
    if (count == 4) {
        // Không bị chặn đầu nào (Open 4): Chắc chắn thắng ở nước tiếp theo vì đối thủ chỉ chặn được 1 đầu.
        if (blocks == 0) return 1000000;
        
        // Bị chặn 1 đầu (Blocked 4): Buộc đối thủ phải chặn đầu còn lại ngay lập tức.
        // Đây là nước đi tấn công mạnh hoặc nước phòng thủ bắt buộc.
        if (blocks == 1) return 100000; 
    }
    
    // TRƯỜNG HỢP 4: Có 3 quân
    if (count == 3) {
        // Không bị chặn đầu nào (Open 3): Rất nguy hiểm, có thể tạo thành Open 4 ở nước sau.
        // Nếu đối thủ không chặn ngay, ta sẽ có tỷ lệ thắng cao.
        if (blocks == 0) return 50000; 
        
        // Bị chặn 1 đầu (Blocked 3): Giá trị thấp hơn nhiều vì đối thủ dễ dàng hóa giải.
        // Tuy nhiên vẫn có giá trị để kiến tạo hoặc dụ đối thủ.
        if (blocks == 1) return 1000;
    }
    
    // TRƯỜNG HỢP 5: Có 2 quân
    if (count == 2) {
        // Open 2: Khởi đầu tốt cho một chuỗi tấn công.
        if (blocks == 0) return 500;
        
        // Blocked 2: Ít giá trị, thường là nước đi phòng thủ thụ động.
        if (blocks == 1) return 50;
    }
    
    // TRƯỜNG HỢP 6: Có 1 quân
    // Điểm khuyến khích để AI ưu tiên đánh gần các quân đã có thay vì đánh lung tung.
    if (count == 1) {
        return 10;
    }
    
    return 0;
}

// Hàm đánh giá nhanh nước đi tại vị trí (x, y) cho người chơi player
long long AI::evaluateMove(Board& board, int x, int y, int player) {
    long long score = 0;
    
    // Giả sử đánh vào ô này
    board.grid[x][y] = player;
    
    int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int d = 0; d < 4; d++) {
        int count = 1; // Tính cả quân vừa đánh
        int block = 0;
        
        // Hướng xuôi
        int i = x + dirs[d][0];
        int j = y + dirs[d][1];
        while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && board.grid[i][j] == player) {
            count++;
            i += dirs[d][0];
            j += dirs[d][1];
        }
        if (i < 0 || i >= BOARD_SIZE || j < 0 || j >= BOARD_SIZE || board.grid[i][j] != EMPTY) block++;
        
        // Hướng ngược
        i = x - dirs[d][0];
        j = y - dirs[d][1];
        while (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && board.grid[i][j] == player) {
            count++;
            i -= dirs[d][0];
            j -= dirs[d][1];
        }
        if (i < 0 || i >= BOARD_SIZE || j < 0 || j >= BOARD_SIZE || board.grid[i][j] != EMPTY) block++;
        
        // Tính điểm cục bộ cho hướng này
        score += evaluateLine(count, block, player == BOT);
    }
    
    // Hoàn tác trả lại ô trống
    board.grid[x][y] = EMPTY;
    
    return score;
}
