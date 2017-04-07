#include <fstream>
#include <iostream>
#include <cassert>
#include <unordered_set>

/******************************************************************************/

typedef unsigned char uc;
typedef unsigned long long ull;

using std::string;
using std::min;
using std::unordered_set;

const int INF = 2147483647;
const ull GOAL = 1147797409030816545;

/******************************************************************************/

struct State {
    inline State();
    inline State(const State &state);

    inline State &operator=(const State &state);

    inline State up() const;
    inline State down() const;
    inline State left() const;
    inline State right() const;

    // Returns heuristic (Linear conflict)
    int heuristic() const;

    // Returns true if goal may be achieved from this state
    bool isSolvable() const;

    // Returns true if state is goal
    bool isGoal() const;

    // Sets plate
    inline void setAt(int place, uc value);

    // Gets plate
    inline uc getAt(int place) const;

    // Stores info about state. 4 bits per one plate
    ull data;
    // Stores position of empty plate
    int nullPos;
    // Stores path to current state
    string path;
};

/******************************************************************************/

const ull AntiMasks[] = {
        0xFFFFFFFFFFFFFFF0, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFF0FF,
        0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFF0FFFF, 0xFFFFFFFFFF0FFFFF,
        0xFFFFFFFFF0FFFFFF, 0xFFFFFFFF0FFFFFFF, 0xFFFFFFF0FFFFFFFF,
        0xFFFFFF0FFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFF0FFFFFFFFFFF,
        0xFFF0FFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF,
        0x0FFFFFFFFFFFFFFF
};

const ull Masks[] = {
        0x000000000000000F, 0x00000000000000F0, 0x0000000000000F00,
        0x000000000000F000, 0x00000000000F0000, 0x0000000000F00000,
        0x000000000F000000, 0x00000000F0000000, 0x0000000F00000000,
        0x000000F000000000, 0x00000F0000000000, 0x0000F00000000000,
        0x000F000000000000, 0x00F0000000000000, 0x0F00000000000000,
        0xF000000000000000
};

/******************************************************************************/

// Function object of IDA* algorithm
class IDAStar {
public:
    // Returns minimal path to goal
    string operator()(const State &start) {
        // Limit of search
        int limit = start.heuristic();
        ans_ = "";
        while (ans_ == "") {
            visited_.emplace(start.data);
            newLimit_ = INF;
            search(start, limit, 1);
            limit = newLimit_;
            visited_.clear();
        }
        return ans_;
    }

private:
    // Expands state. Returns true if goal is achieved and false otherwise
    // g is length of the path to the current state
    bool visit(const State &state, int limit, int g) {
        // Stop search in current branch if goal was achieved
        if (state.isGoal()) {
            if (ans_ == "" || state.path.size() < ans_.size())
                ans_ = state.path;
            return true;
        }

        // If state was not used before
        if (visited_.find(state.data) == visited_.end()) {
            // Visiting it or stopping search if it's further than limit
            // Supposed distance to node
            int f = g + state.heuristic();
            if (f <= limit) {
                // Recursively launching search
                visited_.emplace(state.data);
                search(state, limit, g + 1);
                visited_.erase(state.data);
            } else {
                // Updating limit
                newLimit_ = min(newLimit_, f);
            }
        }
        return false;
    }

    void search(const State &state, int limit, int g) {
        // Expanding any possible state. Stopping search in
        // current branch if goal was achieved
        if (state.nullPos >= 4)
            if (visit(state.up(), limit, g))
                return;
        if (state.nullPos <= 11)
            if (visit(state.down(), limit, g))
                return;
        if (state.nullPos % 4 != 0)
            if (visit(state.left(), limit, g))
                return;
        if (state.nullPos % 4 != 3)
            if (visit(state.right(), limit, g))
                return;
    }

    // Used for updating limit
    int newLimit_;
    // Stores visited in current path states
    unordered_set<ull> visited_;
    // Stores answer if it was found or empty string otherwise
    string ans_;
};

/******************************************************************************/

int main() {
    // Setting initial state
    State start;
    for (int i = 0; i < 16; i++) {
        int x;
        scanf("%d", &x);
        if (x == 0)
            start.nullPos = i;
        start.setAt(i, static_cast<uc>(x));
    }
    if (start.isGoal()) {
        // If it is goal IDA* will not be launched
        printf("%d", 0);
    } else if (start.isSolvable()) {
        // Launching IDA* if initial state is solvable
        IDAStar ida;
        string ans = ida(start);
        printf("%lu\n%s", ans.size(), ans.c_str());
    } else {
        // Printing -1 if it is not solvable
        printf("%d", -1);
    }
    return 0;
}

/****************************STATE*********************************************/

State::State() :
        data(0),
        path("") {}


State::State(const State &state) :
        data(state.data),
        nullPos(state.nullPos),
        path(state.path) {}


void State::setAt(int place, uc value)
{
    data = (data & AntiMasks[place]) |
           (static_cast<ull>(value) << (place << 2));
}


uc State::getAt(int place) const
{
    return static_cast<uc>((data & Masks[place]) >> (place << 2));
}


State State::up() const
{
    assert(nullPos >= 4);
    State result(*this);

    int trg = nullPos - 4;
    uc c = getAt(trg);
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.nullPos = trg;
    result.path += 'D';
    assert(result.path.size() != 0);

    return result;
}


State State::down() const
{
    assert(nullPos <= 11);
    State result(*this);

    int trg = nullPos + 4;
    uc c = getAt(trg);
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.nullPos = trg;
    result.path += 'U';
    assert(result.path.size() != 0);

    return result;
}


State State::left() const
{
    assert(nullPos % 4 != 0);
    State result(*this);

    int trg = nullPos - 1;
    uc c = getAt(trg);
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.nullPos = trg;
    result.path += 'R';
    assert(result.path.size() != 0);

    return result;
}


State State::right() const
{
    assert(nullPos % 4 != 3);
    State result(*this);

    int trg = nullPos + 1;
    uc c = getAt(trg);
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.nullPos = trg;
    result.path += 'L';
    assert(result.path.size() != 0);

    return result;
}


int State::heuristic() const
{
    int heuristic = 0;
    for (int i = 0; i < 16; i++) {
        if (getAt(i) == 0)
            continue;
        heuristic += abs(i % 4 - (getAt(i) - 1) % 4)
                     + abs(i / 4 - (getAt(i) - 1) / 4);
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = j + 1; k < 4; k++) {
                int jp = 4 * i + j;
                int kp = 4 * i + k;
                int jg = getAt(jp) - 1;
                int kg = getAt(kp) - 1;
                if (jp != nullPos && kp != nullPos
                    && jg / 4 == i && kg / 4 == i && jg > kg) {
                    heuristic += 2;
                }
            }
    return heuristic;
}


bool State::isSolvable() const
{
    int inversions = 0;
    for (int i = 0; i < 16; i++)
        for (int j = i + 1; j < 16; j++)
            if (getAt(i) != 0 && getAt(j) != 0 && getAt(i) > getAt(j))
                inversions++;
    return (inversions % 2 == 0) == (nullPos / 4 % 2 == 1);
}


bool State::isGoal() const
{
    return data == GOAL;
}


State &State::operator=(const State &state)
{
    data = state.data;
    nullPos = state.nullPos;
    path = state.path;
    return *this;
}