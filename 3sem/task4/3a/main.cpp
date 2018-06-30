#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>

typedef std::pair<int, int> Square;

Square KW = std::make_pair(3, 3);
const int INF = 1000000000;
const std::vector<std::pair<int, int>> directions {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0),
        std::make_pair(1, 1),
        std::make_pair(1, -1),
        std::make_pair(-1, -1),
        std::make_pair(-1, 1)
};

/*====================================================================================================================*/

bool checkCorrectPos(const Square &position) {
    return (position.first >= 1 && position.first <= 8 && position.second >= 1 && position.second <= 8);
}

bool kingThreat(const Square &K, const Square &position) {
    if (std::abs(K.second - position.second) == 1) {
        return std::abs(K.first - position.first) == 1 || std::abs(K.first - position.first) == 0;
    }
    if (std::abs(K.second - position.second) == 0) {
        return std::abs(K.first - position.first) == 1;
    }
    return false;
}

/*====================================================================================================================*/

struct State {
    Square QW;
    Square KB;
    bool whiteTurn;

    State(Square QW, Square KB, bool whiteTurn)
        : QW(QW)
        , KB(KB)
        , whiteTurn(whiteTurn) {}

    explicit State(int i)
            : QW(((i >= 4096)? i - 4096 : i) / 64 / 8 + 1, ((i >= 4096)? i - 4096 : i) / 64 % 8 + 1)
            , KB(((i >= 4096)? i - 4096 : i) % 64 / 8 + 1, ((i >= 4096)? i - 4096 : i) % 64 % 8 + 1)
            , whiteTurn (i < 4096) {}

    int toInt() const {
        return ((whiteTurn?0:4096) + 64*(8*(QW.first - 1) + QW.second - 1) + 8*(KB.first - 1) + KB.second - 1);
    }

    bool queenThreat() {
        if (QW == KB) return false;
        for (auto direction : directions) {
            for (int i = 1; i <= 8; i++) {
                Square position(QW.first + direction.first * i, QW.second + direction.second * i);
                if (!checkCorrectPos(position)) {
                    continue;
                }
                if (position == KW) {
                    break;
                }
                if (position == KB) {
                    return true;
                }
            }
        }
        return false;
    }

    bool mate() {
        if (!queenThreat()) {
            return false;
        }
        for (auto direction : directions) {
            Square position(KB.first + direction.first, KB.second + direction.second);
            if (!checkCorrectPos(position)) {
                continue;
            }
            State newDesk(QW, position, false);
            if (!newDesk.queenThreat() && !kingThreat(KW, position)) {
                return false;
            }
        }
        return true;
    }

    bool check() {
        return queenThreat() || kingThreat(KW, KB);
    }
};

/*====================================================================================================================*/

class EndgameSolver {
public:
    EndgameSolver() {
        states_.resize(10000);
        for (int p1 = 1; p1 <= 8; p1++) {
            for (int p2 = 1; p2 <= 8; p2++) {
                for (int q1 = 1; q1 <= 8; q1++) {
                    for (int q2 = 1; q2 <= 8; q2++) {
                        State desk1(Square(p1, p2), Square(q1, q2), true);
                        State desk2(Square(p1, p2), Square(q1, q2), false);
                        states_[desk1.toInt()] = INF;
                        states_[desk2.toInt()] = INF;
                    }
                }
            }
        }
    }

    int operator()(State start) {
        std::vector<int> mates = findMates();
        for (int ch : mates) {
            addQueenMoves(State(ch));
        }
        while (!queue_.empty() && states_[start.toInt()] == INF) {
            int cur_id = queue_.front();
            State cur_state(cur_id);
            queue_.pop();
            if (cur_state.whiteTurn) {
                int min_moves = INF;
                if (cur_state.check()) continue;
                for (auto direction : directions) {
                    for (int i = 1; i <= 8; i++) {
                        Square position(cur_state.QW.first + direction.first * i, cur_state.QW.second + direction.second * i);
                        if (!checkCorrectPos(position)) {
                            continue;
                        }
                        if (position == cur_state.KB || position == KW) {
                            break;
                        }
                        State new_state(position, cur_state.KB, !cur_state.whiteTurn);
                        if (kingThreat(cur_state.KB, position) && !kingThreat(KW, position)) {
                            continue;
                        }
                        min_moves = std::min(min_moves, states_[new_state.toInt()]);
                    }
                }
                if (states_[cur_id] > min_moves + 1) {
                    states_[cur_id] = min_moves + 1;
                    addKingMoves(cur_state);
                }
            } else {
                int max_moves = -INF;
                for (auto direction: directions) {
                    Square position(cur_state.KB.first + direction.first, cur_state.KB.second + direction.second);
                    State new_state(cur_state.QW, position, !cur_state.whiteTurn);
                    if (!checkCorrectPos(position)) {
                        continue;
                    }
                    if (position == cur_state.QW || position == KW) {
                        continue;
                    }
                    if (new_state.check()) {
                        continue;
                    }
                    max_moves = std::max(max_moves, states_[new_state.toInt()]);
                }
                if (max_moves != INF) {
                    if (states_[cur_id] == INF || states_[cur_id] < max_moves + 1) {
                        states_[cur_id] = max_moves + 1;
                        addQueenMoves(cur_state);
                    }
                }

            }
        }
        return states_[start.toInt()];
    }

private:

    std::vector<int> findMates() {
        std::vector<int> checkmates;
        for (int i = 1; i <= 5; i++) {
            for (int p1 = 1; p1 <= 8; p1++) {
                for (int p2 = 1; p2 <= 8; p2++) {
                    if (std::make_pair(p1, p2) == std::make_pair(i, 1)) {
                        continue;
                    }
                    if (std::make_pair(p1, p2) == std::make_pair(1, i)) {
                        continue;
                    }
                    if (p1 == KW.first && p2 == KW.first) {
                        continue;
                    }
                    State desk1(Square(p1, p2), Square(1, i), false);
                    if (desk1.mate()) {
                        states_[desk1.toInt()] = 0;
                        checkmates.push_back(desk1.toInt());
                    }
                    if (i != 1) {
                        State desk2(Square(p1, p2), Square(i, 1), false);
                        if (desk2.mate()) {
                            states_[desk2.toInt()] = 0;
                            checkmates.push_back(desk2.toInt());
                        }
                    }
                }
            }
        }
        return checkmates;
    }

    void addQueenMoves(State state) {
        if (state.whiteTurn) return;
        for (auto direction : directions) {
            for (int i = 1; i <= 8; i++) {
                Square position(state.QW.first + direction.first * i, state.QW.second + direction.second * i);
                if (!checkCorrectPos(position)) {
                    continue;
                }
                if (position == state.KB || position == KW) {
                    break;
                }
                State newDesk(Square(position.first, position.second), state.KB, !state.whiteTurn);
                if (newDesk.check()) {
                    continue;
                }
                if (states_[newDesk.toInt()] != INF && states_[newDesk.toInt()] <= states_[state.toInt()] + 1) {
                    continue;
                }
                queue_.push(newDesk.toInt());
            }
        }
    }

    void addKingMoves(State state) {
        if (!state.whiteTurn) {
            return;
        }
        for (auto direction: directions) {
            Square position(state.KB.first + direction.first, state.KB.second + direction.second);
            State newDesk(state.QW, position, !state.whiteTurn);
            if (!checkCorrectPos(position)) {
                continue;
            }
            if (position == state.QW || position == KW) {
                continue;
            }
            if (kingThreat(KW, position)) {
                continue;
            }
            if (states_[newDesk.toInt()] != INF && states_[newDesk.toInt()] >= states_[state.toInt()] + 1) {
                continue;
            }
            queue_.push(newDesk.toInt());
        }
    }


    std::vector<int> states_;
    std::queue<int> queue_;
};

/*====================================================================================================================*/

int main() {
    char qx, qy, kx, ky;
    std::cin >> qx >> qy >> kx >> ky;
    State start(Square(qx - 'a' + 1, qy - '0'), Square(kx - 'a' + 1, ky - '0'), true);
    EndgameSolver solve;
    int result = solve(start);
    if (result == INF) {
        std::cout << "IMPOSSIBLE";
    } else {
        std::cout << result;
    }
    return 0;
}