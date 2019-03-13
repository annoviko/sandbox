struct cross {
public:
    std::size_t i = 0;
    std::stack<std::pair<std::size_t, std::size_t>> next;

public:
    cross() = default;
    cross(std::size_t _y, std::size_t _x, std::size_t _i) :
        i(_i), next()
    { 
        next.push({ _y, _x });
    }
};

class Solution {
private:
    std::vector<std::vector<bool>> visited;

public:
    bool exist(std::vector<std::vector<char>>& board, std::string word) {
        if (word.empty()) { return true; }
        if (board.size() * board[0].size() < word.size()) { return false; }

        visited = std::vector<std::vector<bool>>(board.size(), std::vector<bool>(board[0].size()));

        for (std::size_t i = 0; i < board.size(); i++) {
            for (std::size_t j = 0; j < board[0].size(); j++) {
                if (word[0] == board[i][j] && traverse(board, i, j, word, 1)) {
                    return true;
                }
            }
        }
        
        return false;
    }

    bool traverse(std::vector<std::vector<char>>& board, std::size_t y, std::size_t x, std::string & word, std::size_t index) {
        if (index == word.size()) {
            return true; 
        }

        visited[y][x] = true;

        const std::size_t ylim = board.size();
        const std::size_t xlim = board[0].size();

        const char symbol = word[index];

        if (y > 0 && (board[y - 1][x] == symbol) && !visited[y - 1][x]) {
            if (traverse(board, y - 1, x, word, index + 1)) {
                return true; 
            }
        }
            
        if (y + 1 < ylim && (board[y + 1][x] == symbol) && !visited[y + 1][x]) {
            if (traverse(board, y + 1, x, word, index + 1)) {
                return true; 
            }
        }
            
        if (x > 0 && (board[y][x - 1] == symbol) && !visited[y][x - 1]) {
            if (traverse(board, y, x - 1, word, index + 1)) {
                return true; 
            }
        }

        if (x + 1 < xlim && (board[y][x + 1] == symbol) && !visited[y][x + 1]) {
            if (traverse(board, y, x + 1, word, index + 1)) {
                return true; 
            }
        }

        visited[y][x] = false;
        return false;
    }
};