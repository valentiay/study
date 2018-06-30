#include <iostream>
#include <vector>
#include <queue>

struct State {
    /**
     *     A B C D E F G H
     *     0 1 2 3 4 5 6 7
     *    0
     *    1  ######
     *    2  ##KW##
     *    3  ######
     *    4
     *    5
     *    6
     *    7
     *
     */

    struct Square {
        Square(int x, int y)
            : x(x)
            , y(y) {}

        int x;
        int y;
    };

    Square kingPos;
    Square queenPos;
    int id;
    bool whiteTurn;

    State(Square kingPos, Square queenPos, bool whiteTurn)
            : kingPos(kingPos)
            , queenPos(queenPos)
            , id(getId(kingPos, queenPos, whiteTurn))
            , whiteTurn(whiteTurn){}

    explicit State(int id)
            : kingPos(((id >= 4096)? id - 4096 : id) % 64 % 8, ((id >= 4096)? id - 4096 : id) % 64 / 8)
            , queenPos(((id >= 4096)? id - 4096 : id) / 64 % 8, ((id >= 4096)? id - 4096 : id) / 64 / 8)
            , id(id)
            , whiteTurn(id < 4096) {}

    bool mate() {
        if (whiteTurn) {
            return false;
        }
        if (kingPos.x > 0) {
            if (kingPos.y > 0 && !blackLockedPosition(kingPos.x - 1, kingPos.y - 1)) return false;
            if (!blackLockedPosition(kingPos.x - 1, kingPos.y)) return false;
            if (kingPos.y < 7 && !blackLockedPosition(kingPos.x - 1, kingPos.y + 1)) return false;
        }
        if (kingPos.y > 0 && !blackLockedPosition(kingPos.x, kingPos.y - 1)) return false;
        if (kingPos.y < 7 && !blackLockedPosition(kingPos.x, kingPos.y + 1)) return false;
        if (kingPos.x < 7) {
            if (kingPos.y > 0 && !blackLockedPosition(kingPos.x + 1, kingPos.y - 1)) return false;
            if (!blackLockedPosition(kingPos.x + 1, kingPos.y)) return false;
            if (kingPos.y < 7 && !blackLockedPosition(kingPos.x + 1, kingPos.y + 1)) return false;
        }
        return true;
    }

    bool blackLockedPosition(int x, int y) {
        if (1 <= x && x <= 3 && 1 <= y && y <= 3) {
            return true;
        }

        if (x == queenPos.x && (x != 2 || (y - 2)*(queenPos.y - 2) >= 0)
            || y == queenPos.y && (y != 2 || (x - 2)*(queenPos.x - 2)) >= 0)
        {
            return true;
        }

        return abs(x - queenPos.x) == abs(y - queenPos.y)
               && ((abs(x - 2) != abs(y - 2) || abs(queenPos.x - 2) != abs(queenPos.y - 2))
               || (x - 2)*(queenPos.x - 2) >= 0);
    }

    bool whiteLockedPosition(int x, int y) {
        if (abs(x - 2) <= 1 && abs(y - 2) <= 1 ) return false;
        return abs(kingPos.x - x) <= 1 && abs(kingPos.y - y) <= 1;
    }

    std::vector<int> getNextStates() {
        std::vector<int> availableIds;
        if (whiteTurn) {
            for (Square p(queenPos.x, queenPos.y - 1); p.y >= 0; p.y--) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x + 1, queenPos.y - 1); p.x < 8 && p.y >= 0; p.x++, p.y--) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x + 1, queenPos.y); p.x < 8; p.x++) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x + 1, queenPos.y + 1); p.x < 8 && p.y < 8; p.x++, p.y++) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x, queenPos.y + 1); p.y < 8; p.y++) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x - 1, queenPos.y + 1); p.x >= 0 && p.y < 8; p.x--, p.y++) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x - 1, queenPos.y); p.x >= 0; p.x--) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
            for (Square p(queenPos.x - 1, queenPos.y - 1); p.x >= 0 && p.y >= 0; p.x--, p.y--) {
                if (p.x == 2 && p.y == 2) break;
                if (whiteLockedPosition(p.x, p.y)) break;
                if (p.x == kingPos.x && p.y == kingPos.y) break; // Should never happen
                availableIds.push_back(getId(kingPos, p, false));
            }
        } else {
            if (kingPos.x > 0) {
                if (kingPos.y > 0 && !blackLockedPosition(kingPos.x - 1, kingPos.y - 1)) {
                    availableIds.push_back(getId(Square(kingPos.x - 1, kingPos.y - 1), queenPos, true));
                }
                if (!blackLockedPosition(kingPos.x - 1, kingPos.y)) {
                    availableIds.push_back(getId(Square(kingPos.x - 1, kingPos.y), queenPos, true));
                }
                if (kingPos.y < 7 && !blackLockedPosition(kingPos.x - 1, kingPos.y + 1)) {
                    availableIds.push_back(getId(Square(kingPos.x - 1, kingPos.y + 1), queenPos, true));
                }
            }
            if (kingPos.y > 0 && !blackLockedPosition(kingPos.x, kingPos.y - 1)) {
                availableIds.push_back(getId(Square(kingPos.x, kingPos.y - 1), queenPos, true));
            }
            if (kingPos.y < 7 && !blackLockedPosition(kingPos.x, kingPos.y + 1)) {
                availableIds.push_back(getId(Square(kingPos.x, kingPos.y + 1), queenPos, true));
            }
            if (kingPos.x < 7) {
                if (kingPos.y > 0 && !blackLockedPosition(kingPos.x + 1, kingPos.y - 1)) {
                    availableIds.push_back(getId(Square(kingPos.x + 1, kingPos.y - 1), queenPos, true));
                }
                if (!blackLockedPosition(kingPos.x + 1, kingPos.y)) {
                    availableIds.push_back(getId(Square(kingPos.x + 1, kingPos.y), queenPos, true));
                }
                if (kingPos.y < 7 && !blackLockedPosition(kingPos.x + 1, kingPos.y + 1)) {
                    availableIds.push_back(getId(Square(kingPos.x + 1, kingPos.y + 1), queenPos, true));
                }
            }
        }
        return availableIds;
    }

    friend const std::ostream & operator<<(std::ostream & os, State s) {
        os << "\n  A B C D E F G H\n";
        for (int i = 0; i < 8; i++) {
            os << i << " ";
            for (int j = 0; j < 8; j++) {
                if (s.kingPos.x == j && s.kingPos.y == i) {
                    os << "KB";
                } else if (s.queenPos.x == j && s.queenPos.y == i) {
                    os << "QW";
                } else if (i == 2 && j == 2) {
                    os << "KW";
                } else {
                    os << "  ";
                }
            }
            os << "\n";
        }
    }

    static int getId(Square kingPos, Square queenPos, bool whiteTurn) {
        return ((whiteTurn)? 0 : 4096) + (queenPos.y * 8 + queenPos.x) * 64 + kingPos.y * 8 + kingPos.x;
    }
};

int turnsToMate(State::Square KB, State::Square QW) {
    State current_state(KB, QW, true);
    std::queue<std::pair<int, int>> queue;
    std::vector<bool> used(8192, false);
    queue.push(std::make_pair(current_state.id, 0));
    used[current_state.id] = true;
    while(!queue.empty()) {
        std::pair<int, int> current = queue.front();
        queue.pop();
        current_state = State(current.first);
        if (current_state.mate()) {
            return current.second;
        }
        std::vector<int> next_state_ids = current_state.getNextStates();
        for (int id : next_state_ids) {
            if (!used[id]) {
                std::cout << current.first << " -> " << id << "\n";
                used[id] = true;
                queue.push(std::make_pair(id, current.second + 1));
            }
        }
    }
    return -1;
}

int main() {
    std::string KB_raw, QW_raw;
    std::cin >> QW_raw >> KB_raw;
    State::Square KB(KB_raw[0] - 'a', KB_raw[1] - '1');
    State::Square QW(QW_raw[0] - 'a', QW_raw[1] - '1');
    std::cout << turnsToMate(KB, QW);

//    std::cout << State(1413);
//    State s(5573);
//    std::cout << State(5573);
//    std::cout << State(1476);
//    std::cout << State(5444);
//    std::cout << s.blackLockedPosition(4, 0);
}